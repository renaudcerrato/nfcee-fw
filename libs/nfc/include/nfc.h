#ifndef NFC_H
#define NFC_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    NFC_DIGITAL_RF_14443A = 0,
    NFC_DIGITAL_RF_14443B,
    NFC_DIGITAL_RF_15693,
    NFC_DIGITAL_RF_FELICA,
} nfc_digital_t;

struct nfc_tx {
    void                *buf;
    size_t              bits;
    struct nfc_tx       *next;
};

struct nfc_rx {
    void                *buf;
    size_t              len;
};

typedef enum {
    NFC_DEVICE_IOCW_SWITCH_RF = 0,
    NFC_DEVICE_IOCR_FRAME_SIZE,
} nfc_device_req_t;

struct nfc_device_fops {
    int (*open)(void *, nfc_digital_t tech);
    int (*transceive)(void *, const struct nfc_tx *tx, const struct nfc_rx *rx, unsigned int timeout);
    int (*ioctl)(void *, nfc_device_req_t, void *);
    int (*close)(void *);
};

struct nfc_device {
    void *priv;
    struct nfc_device_fops ops;
};

#define _nfc_driver_data            struct nfc_device       *dev;       \
                                    nfc_digital_t           tech;
typedef struct {
    _nfc_driver_data
} nfc_driver_t;

int nfc_open(nfc_driver_t *driver);
int nfc_transceive(nfc_driver_t *driver, const struct nfc_tx *tx, const struct nfc_rx *rx, unsigned int timeout);
int nfc_close(nfc_driver_t *driver);

#include "iso14443.h"

#endif //NFC_H
