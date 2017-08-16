#include <nfc.h>
#include <errno.h>

int nfc_open(nfc_driver_t *driver) {
    switch(driver->tech) {
        case NFC_DIGITAL_RF_14443A:
        case NFC_DIGITAL_RF_14443B:
            return nfc_iso14443_open((nfc_iso14443_driver_t *) driver);
        default:
            return -EINVAL;
    }
}

int nfc_transceive(nfc_driver_t *driver, const void *tx, size_t txlen, void *rx, size_t rxlen, unsigned int timeout) {
    switch(driver->tech) {
        case NFC_DIGITAL_RF_14443A:
        case NFC_DIGITAL_RF_14443B:
            return nfc_iso14443_transceive((nfc_iso14443_driver_t *) driver, tx, txlen, rx, rxlen, timeout);
        default:
            return -EINVAL;
    }
}

int nfc_close(nfc_driver_t *driver) {
    switch(driver->tech) {
        case NFC_DIGITAL_RF_14443A:
        case NFC_DIGITAL_RF_14443B:
            return nfc_iso14443_close((nfc_iso14443_driver_t *) driver);
        default:
            return -EINVAL;
    }
}