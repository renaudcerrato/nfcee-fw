#include <trf797x.h>
#include "trf797x_lld.h"

#if TRF797X_CONF_HAVE_LIBNFC

static int nfc_device_open(struct trf797x_nfc_device *dev, nfc_digital_t tech) {

    switch(tech) {
        case NFC_DIGITAL_RF_14443A:
            dev->config.proto = TRF7970X_PROTO_RFID_14443A_106;
            break;
        case NFC_DIGITAL_RF_14443B:
            dev->config.proto = TRF7970X_PROTO_RFID_14443B_106;
            break;
        case NFC_DIGITAL_RF_15693:
            dev->config.proto = TRF7970X_PROTO_RFID_15693_SGL_1OF4_2648;
            break;
        case NFC_DIGITAL_RF_FELICA:
            dev->config.proto = TRF7970X_PROTO_RFID_FELICA_212;
            break;
        default:
            return -EINVAL;
    }

    return trf797x_initiator_start(&dev->driver, &dev->config);
}

static int nfc_device_transceive(struct trf797x_nfc_device *dev, const struct nfc_iovec *tx, size_t len, struct nfc_iovec *rx, unsigned int timeout) {
    struct trf797x_iovec _tx = { tx->base, .bits = tx->bits };
    struct trf797x_iovec _rx = { rx->base, .bits = rx->bits };

    return trf797x_initiator_transceive(&dev->driver, &_tx, len, &_rx, MS2ST(timeout));
}

static int nfc_device_ioctl(struct trf797x_nfc_device *dev, nfc_iocreq_t req, void *arg) {

    switch(req) {
        case NFC_IOCR_DEV_FRAME_SIZE:
            *((size_t *) arg) = TRF797X_FIFO_SIZE;
            break;
        case NFC_IOCW_SWITCH_RF:
            return trf797x_switch_rf((Trf797xDriver *) &dev->driver, ((int) arg) ? TRUE : FALSE);
        default:
            return -EINVAL;
    }

    return 0;
}

static int nfc_device_close(struct trf797x_nfc_device *dev) {
    return trf797x_initiator_stop(&dev->driver, FALSE);
}

void trf797x_nfc_device_init(struct trf797x_nfc_device *device) {

    device->open = (typeof(device->open)) nfc_device_open;
    device->transceive = (typeof(device->transceive)) nfc_device_transceive;
    device->ioctl = (typeof(device->ioctl)) nfc_device_ioctl;
    device->close = (typeof(device->close)) nfc_device_close;

    trf797x_initiator_driver_init(&device->driver);
}

#endif // TRF797X_CONF_HAVE_LIBNFC