#include <nfc.h>
#include <errno.h>

int nfc_iso14443a_open(nfc_iso14443a_driver_t *driver);
int nfc_iso14443a_transceive(nfc_iso14443a_driver_t *driver, const struct nfc_tx *tx, const struct nfc_rx *rx, unsigned int timeout);
int nfc_iso14443a_close(nfc_iso14443a_driver_t *driver);

#define isIBLOCK(b)		((b & 0xE2) == 0x02)
#define isRBLOCK(b)		((b & 0xE6) == 0xA2)
#define isSBLOCK(b)		((b & 0xC7) == 0xC2)

#define isACK(b)		(isRBLOCK(b) && !(b & 0x10))
#define isWTX(b)		(isSBLOCK(b) && ((b & 0x30) == 0x30))

#define fsdi2fsd	fsci2fsc
#define fsc2fsci	fsd2fsdi

#ifndef MIN
#define MIN(a,b)        ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)        ((b) < (a) ? (a) : (b))
#endif

static int fsci2fsc(int fsci) {
    const uint16_t lut[] = { 16, 24, 32, 40, 48, 64, 96, 128, 256};
    return lut[MIN(fsci, sizeof(lut)/sizeof(lut[0]) - 1)];
}

static int fsd2fsdi(int fsd) {
    static const uint16_t lut[] = { 16, 24, 32, 40, 48, 64, 96, 128, 256};
    static const int size = sizeof(lut)/sizeof(lut[0]);

    for(int i = size - 1; i >= 0; i--) {
        if (lut[i] <= fsd) {
            return i;
        }
    }

    return 0;
}

static int fwi2fw(int fwi) {
    const uint16_t lut[] = { 1, 1, 2, 3, 5, 10, 20, 40, 80, 160, 310, 620, 1240, 2480, 4950};
    return lut[MIN(fwi, sizeof(lut)/sizeof(lut[0]) - 1)];
}

static int fw2fwi(int fw) {
    static const uint16_t lut[] = { 1, 1, 2, 3, 5, 10, 20, 40, 80, 160, 310, 620, 1240, 2480, 4950};
    static const int size = sizeof(lut)/sizeof(lut[0]);

    for(int i = 0; i < size; i++) {
        if (lut[i] >= fw)
            return i;
    }

    return lut[size - 1];
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

void nfc_iso14443a_driver_init(nfc_iso14443a_driver_t *driver, struct nfc_device *dev) {
    driver->dev = dev;
    driver->tech = NFC_DIGITAL_RF_14443A;
    driver->l3.open = (typeof(driver->l3.open)) nfc_iso14443a_open;
    driver->l3.transceive = (typeof(driver->l3.transceive)) nfc_iso14443a_transceive;
    driver->l3.close = (typeof(driver->l3.close)) nfc_iso14443a_close;
}


int nfc_iso14443_open(nfc_iso14443_driver_t *driver) {

    int fwi = 8;
    int ret = driver->l3.open(driver);

    if(ret < 0 || !is_tcl(driver)) {
        return ret;
    }

    switch(driver->tech) {

        case NFC_DIGITAL_RF_14443A: {
            uint8_t rats[] = { 0xE0, fsd2fsdi(layer->fsd) << 4 };
            uint8_t ats[7];

            struct nfc_tx tx = { .buf = rats, .bits = sizeof(rats)*8, .next = NULL};
            struct nfc_rx rx = { .buf = ats, .len = sizeof(ats)};

            // Send RATS
            if(driver->l3.transceive(driver, &tx, &rx, 100) < 0) {
                goto error;
            }

            if(ats[0] > 1) {
                driver->fsc = fsci2fsc(ats[1] & 0x0F);

                if(ats[1] & 0x20) {
                    fwi = ats[(ats[1] & 0x10) ? 3 : 2] >> 4;
                }
            }
        }
            break;

        case NFC_DIGITAL_RF_14443B: {
            uint8_t *atqb = ((nfc_iso14443b_driver_t *)driver)->atqb;
            driver->fsc = fsci2fsc(atqb[10] >> 4);
            fwi = atqb[11] >> 4;
        }
            break;

        default:
            goto error;
    }

    driver->fwt = MAX(100, fwi2fw(fwi));
    driver->block = 0;

    return 0;
error:
    driver->l3.close(driver);
    return -EIO;
}

int nfc_iso14443_transceive(nfc_iso14443_driver_t *driver, const struct nfc_tx *tx, const struct nfc_rx *rx, unsigned int timeout) {

    if(!is_tcl(driver)) {
        return driver->l3.transceive(driver, tx, rx, timeout);
    }

    //TODO

    return -EINVAL;
}

int nfc_iso14443_close(nfc_iso14443_driver_t *driver) {

    if(!is_tcl(driver)) {
        return driver->l3.close(driver);
    }

    //TODO

    return -EINVAL;
}
