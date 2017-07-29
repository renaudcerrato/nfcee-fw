#include <nfc.h>
#include <errno.h>
#include <string.h>

#define DEFAULT_TIMEOUT             15

#define is_bcc_valid(c, bcc)		((((uint8_t *)(c))[0]^((uint8_t *)(c))[1]^                  \
                                        ((uint8_t *)(c))[2]^((uint8_t *)(c))[3]) == (bcc))

static uint16_t crca(const uint8_t data[], size_t len) {

    uint16_t crc = 0x6363;

    while(len--) {
        uint8_t ch = *(data++);
        ch = (ch ^ crc);
        ch = (ch ^(ch << 4));
        crc = (crc >> 8) ^ ((uint16_t)ch << 8) ^ ((uint16_t)ch << 3) ^ (ch >> 4);
    }

    return crc;
}

static int transceive(struct nfc_device *dev, const void *tx, size_t txlen, void *rx, size_t rxlen, unsigned int timeout) {
    struct nfc_tx nfc_tx = {.buf = tx, .bits = 8*txlen, .next = NULL};
    struct nfc_rx nfc_rx = {.buf = rx, .len = rxlen};
    return dev->ops.transceive(dev->priv, &nfc_tx, &nfc_rx, timeout);
}

static int transceive_short(struct nfc_device *dev, uint8_t  tx, void *rx, size_t rxlen, unsigned int timeout) {
    struct nfc_tx nfc_tx = {.buf = &tx, .bits = 7, .next = NULL};
    struct nfc_rx nfc_rx = {.buf = rx, .len = rxlen};
    return dev->ops.transceive(dev->priv, &nfc_tx, &nfc_rx, timeout);
}

int nfc_iso14443a_transceive(nfc_iso14443a_driver_t *driver, const void *tx, size_t txlen, void *rx, size_t rxlen, unsigned int timeout) {

    uint16_t c = crca(tx, txlen);
    uint8_t crc[]= {c & 0xff, c >> 8};

    struct nfc_tx nfc_crc = {.buf = crc, .bits = 8*sizeof(crc), .next = NULL};
    struct nfc_tx nfc_tx = {.buf = tx, .bits = 8*txlen, .next = &nfc_crc};
    struct nfc_rx nfc_rx = {.buf = rx, .len = rxlen};

    return driver->dev->ops.transceive(driver->dev->priv, &nfc_tx, &nfc_rx, timeout);
}

int nfc_iso14443a_open(nfc_iso14443a_driver_t *driver) {
    uint8_t buf[8], size, sak;
    int ret = 0;

    // Send WUPA, read ATQA
    if(transceive_short(driver->dev, 0x52, buf, sizeof(buf), DEFAULT_TIMEOUT) < 0) {
        ret = -ENODEV;
        goto err;
    }

    // Save ATQA
    driver->atqa = buf[0] | (buf[1] << 8);

    // Not 14443-3 compliant
    if(!(buf[0] & 0x1F)) {
        ret = -EPROTO;
        goto err;
    }

    // UID size
    size = buf[0] >> 6;

    // sanity check
    if(size > 2) {
        ret = -EIO;
        goto err;
    }

    /**** TODO: optimize that ugly loop! */
    // Retrieve UID CL1
    buf[0] = 0x93; buf[1] = 0x20;
    if(transceive(driver->dev, buf, 2, buf + 2, 5, DEFAULT_TIMEOUT) < 0) {
        ret = -EIO;
        goto err;
    }

    /* Check BCC */
    if(!is_bcc_valid(buf + 2, buf[2 + 4])) {
        ret = -EIO;
        goto err;
    }

    memcpy(driver->nfcid.data, buf + 2, 4);
    driver->nfcid.len =  4;

    /* Handle cascade level */
    while(size--)
    {
        // Close cascade level n
        buf[1] = 0x70;

        if(nfc_iso14443a_transceive(driver, buf, 7, &sak, 1, DEFAULT_TIMEOUT) < 0) {
            ret = -EIO;
            goto err;
        }

        // Cascade level n+1, retrieve UID CLn
        buf[0]+=2; buf[1] = 0x20;

        if(transceive(driver->dev, buf, 2, buf + 2, 5, DEFAULT_TIMEOUT) < 0) {
            ret = -EIO;
            goto err;
        }
        
        // Check BCC
        if(!is_bcc_valid(buf + 2, buf[2 + 4])) {
            ret = -EIO;
            goto err;
        }

        memmove(driver->nfcid.data + (driver->nfcid.len - 4), driver->nfcid.data + (driver->nfcid.len - 4) + 1, 3);
        memcpy(driver->nfcid.data + driver->nfcid.len - 1, buf + 2, 4);
        driver->nfcid.len += 3;
    }

    // Close cascade level n
    buf[1] = 0x70;

    if(nfc_iso14443a_transceive(driver, buf, 7, buf, 3, DEFAULT_TIMEOUT) < 0) {
        ret = -EIO;
        goto err;
    }

    // Check CRC
    if(crca(buf, 1) != (buf[1] + (buf[2] << 8))) {
        ret = -EIO;
        goto err;
    }

    driver->sak = buf[0];
    return 0;
err:
    return ret;
}

int nfc_iso14443a_close(nfc_iso14443a_driver_t *driver) {
    const uint8_t halt[] = {0x50, 0x00};
    return nfc_iso14443a_transceive(driver, halt, sizeof(halt), NULL, 0, 0);
}

