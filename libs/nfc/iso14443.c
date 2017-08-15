#include <nfc.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int nfc_iso14443a_open(nfc_iso14443a_driver_t *driver);
int nfc_iso14443a_transceive(nfc_iso14443a_driver_t *driver, const void *tx, size_t txlen, void *rx, size_t rxlen, unsigned int timeout);
int nfc_iso14443a_close(nfc_iso14443a_driver_t *driver);

#define MAX_FRAME_SIZE      256

#define is_iblock(b)		(((b) & 0xE2) == 0x02)
#define is_rblock(b)		(((b) & 0xE6) == 0xA2)
#define is_sblock(b)		(((b) & 0xC7) == 0xC2)

#define is_ack(b)		    (is_rblock(b) && !((b) & 0x10))
#define is_wtx(b)		    (is_sblock(b) && (((b) & 0x30) == 0x30))

#define togglebit(b)        ((uint8_t)((b) & 1))

#ifndef MIN
#define MIN(a,b)            ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)            ((b) < (a) ? (a) : (b))
#endif

size_t fsci2fsc(uint8_t fsci) {
    const uint16_t lut[] = { 16, 24, 32, 40, 48, 64, 96, 128, 256};
    return lut[MIN(fsci, sizeof(lut)/sizeof(lut[0]) - 1)];
}

uint8_t fsd2fsdi(int fsd) {
    static const uint16_t lut[] = { 16, 24, 32, 40, 48, 64, 96, 128, 256};
    static const int size = sizeof(lut)/sizeof(lut[0]);

    for(int i = size - 1; i >= 0; i--) {
        if (lut[i] <= fsd) {
            return (uint8_t) i;
        }
    }

    return 0;
}

size_t fwi2fw(uint8_t fwi) {
    const uint16_t lut[] = { 1, 1, 2, 3, 5, 10, 20, 40, 80, 160, 310, 620, 1240, 2480, 4950};
    return lut[MIN(fwi, sizeof(lut)/sizeof(lut[0]) - 1)];
}

static bool is_tcl(nfc_iso14443_driver_t *driver) {
    switch(driver->tech) {
        case NFC_DIGITAL_RF_14443A:
            return (((nfc_iso14443a_driver_t *)driver)->sak & 0x24) == 0x20;
        case NFC_DIGITAL_RF_14443B:
            return (((nfc_iso14443b_driver_t *)driver)->atqb[10] & 0x09) == 0x01;
        default:
            return 0;
    }
}

nfc_driver_t * nfc_iso14443a_driver_init(nfc_iso14443a_driver_t *driver, struct nfc_device *dev) {
    driver->dev = dev;
    driver->state = ISO14443_STATE_POWER_OFF;
    driver->tech = NFC_DIGITAL_RF_14443A;
    driver->l3.open = (typeof(driver->l3.open)) nfc_iso14443a_open;
    driver->l3.transceive = (typeof(driver->l3.transceive)) nfc_iso14443a_transceive;
    driver->l3.close = (typeof(driver->l3.close)) nfc_iso14443a_close;
    return (nfc_driver_t *) driver;
}

int nfc_iso14443_open(nfc_iso14443_driver_t *driver) {

    int ret;

    if(driver->state == ISO14443_STATE_POWER_OFF) {

        if((ret = driver->dev->open(driver->dev, driver->tech)) != 0) {
            return ret;
        }

        // Turn RF on
        if((ret = driver->dev->ioctl(driver->dev, NFC_IOCW_SWITCH_RF, (void *) 1)) != 0) {
            goto error;
        }

        driver->dev->ioctl(driver->dev, NFC_IOCR_DEV_FRAME_SIZE, &driver->fsd);

        usleep(6000L);
        driver->state = ISO14443_STATE_IDLE;
    }

    if(driver->state == ISO14443_STATE_IDLE || driver->state == ISO14443_STATE_HALTED) {

        if((ret = driver->l3.open(driver)) != 0) {
            goto error;
        }

        driver->state = ISO14443_STATE_SELECTED;
        driver->block = 0;
    }

    return 0;
error:
    driver->dev->close(driver->dev);
    driver->state = ISO14443_STATE_POWER_OFF;
    return ret;
}

int nfc_iso14443_transceive(nfc_iso14443_driver_t *driver, const void *txdata, size_t txlen, void *rxdata, size_t rxlen, unsigned int timeout) {

    if(!is_tcl(driver)) {
        return driver->l3.transceive(driver, txdata, txlen, rxdata, rxlen, timeout);
    }

    uint8_t buffer[MAX_FRAME_SIZE];
    size_t inf_size, received;

    // INF field maximum size (no NAD, no CID)
    inf_size = MIN(driver->fsd, MIN(driver->fsc, sizeof buffer)) - 3;

    while(txlen > inf_size) {

        // I-Block : chaining
        buffer[0] = 0x12 | togglebit(driver->block);

        // Push (FSC - 3) bytes of data (no NAD, no CID)
        memcpy(buffer + 1, txdata, inf_size);

        do {
            if(driver->l3.transceive(driver, buffer, inf_size + 1, buffer, 3, driver->fwt) < 3)
                goto error;

            // I-Blocks are not allowed during chaining
            if(is_iblock(buffer[0])) {
                goto error;
            }

            // More time requested ? (Rule 9)
            while(is_wtx(buffer[0]))
            {
                // Rule 3
                buffer[1] &= 0x3F;

                if(driver->l3.transceive(driver, buffer, 2, buffer, 3, driver->fwt * buffer[1]) < 3) {
                    goto error;
                }
            }

            if(!is_ack(buffer[0])) {
                goto error;
            }

        }while(togglebit(buffer[0]) != togglebit(driver->block)); // Rule 6/7

        txdata+=inf_size;
        txlen-=inf_size;

        // Rule B
        driver->block++;
    }

    // Send last block and retrieve answer
    buffer[0] = 0x02;
    memcpy(buffer + 1, txdata, txlen);

    received = 0;

    do {
        int i, size_of_header;

        buffer[0] |= togglebit(driver->block);

        if((i = driver->l3.transceive(driver, buffer, txlen + 1, buffer, sizeof buffer, driver->fwt)) < 3) {
            goto error;
        }

        // More time requested ? (Rule 9)
        while(is_wtx(buffer[0])) {

            // Rule 3
            buffer[1] &= 0x3F;

            if((i = driver->l3.transceive(driver, buffer, 2, buffer, sizeof buffer, driver->fwt * buffer[1])) < 3) {
                goto error;
            }
        }

        if(!is_iblock(buffer[0])) {
            goto error;
        }

        size_of_header = 1;

        // CID present ?
        if(buffer[0] & 0x08)
            size_of_header++;

        // NAD present ?
        if(buffer[0] & 0x04) {
            size_of_header++;
        }

        // Read data from FIFO
        if(togglebit(buffer[0]) == togglebit(driver->block))
        {
            inf_size = MIN(i - size_of_header - 2, rxlen);
            memcpy(rxdata, buffer + size_of_header, inf_size);
            rxdata+=inf_size;
            rxlen-=inf_size;
            received+=inf_size;
        }

        // Rule B
        driver->block++;

        // Chaining used ?
        if(!(buffer[0] & 0x10)) {
            break;
        }

        // Prepare ACK
        buffer[0] = 0xA2;
        txlen = 0;

    }while(1);

    return received;
error:
    return -EIO;
}

int nfc_iso14443_ioctl(nfc_iso14443_driver_t *driver, nfc_iocreq_t req, void *arg) {

    int ret = driver->dev->ioctl(driver->dev, req, arg);

    if(req == NFC_IOCW_SWITCH_RF && !(int) arg) {
        driver->state = ISO14443_STATE_POWER_OFF;
    }

    return ret;
}

/**
 * TODO: support multiple PICC in field.
 * Since driver->dev->close() shutdown the RF (as of now),
 * there's no point to send a DESELECT.
 */
int nfc_iso14443_close(nfc_iso14443_driver_t *driver) {

#if 0
    int ret = 0;

    if(driver->state == ISO14443_STATE_SELECTED) {

        if (is_tcl(driver)) {
            uint8_t deselect = 0xC2, answer;

            if ((ret = driver->l3.transceive(driver, &deselect, 1, &answer, 1, 5)) == 1) {
                ret = answer == deselect ? 0 : -EIO;
            }
        } else {
            ret = driver->l3.close(driver);
        }
    }
#endif

    driver->state = ISO14443_STATE_POWER_OFF;
    return driver->dev->close(driver->dev);
}
