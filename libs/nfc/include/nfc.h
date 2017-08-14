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

struct nfc_iovec {
    void                *base;
    union {
        uint32_t        bits;
        struct {
#ifndef __BYTE_ORDER__
#error "you must define __BYTE_ORDER__"
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            uint32_t    :3;
            uint32_t    bytes:29;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            uint32_t    bytes:29;
            uint32_t    :3;
#else
#error "unsupported endian"
#endif
        };
    };
};

typedef enum {
    NFC_IOCW_SWITCH_RF = 0,
    NFC_IOCR_DEV_FRAME_SIZE,
} nfc_iocreq_t;

#define _nfc_device_data            int (*open)(void *, nfc_digital_t tech);            \
                                    int (*transceive)(void *, const struct nfc_iovec *tx, size_t len, struct nfc_iovec *rx, unsigned int timeout);  \
                                    int (*ioctl)(void *, nfc_iocreq_t, void *);         \
                                    int (*close)(void *);

struct nfc_device {
    _nfc_device_data
};

#define _nfc_driver_data            struct nfc_device       *dev;       \
                                    nfc_digital_t           tech;
typedef struct {
    _nfc_driver_data
} nfc_driver_t;

int nfc_open(nfc_driver_t *driver);
int nfc_transceive(nfc_driver_t *driver, const void *tx, size_t txlen, void *rx, size_t rxlen, unsigned int timeout);
int nfc_ioctl(nfc_driver_t *driver, nfc_iocreq_t req, void *arg);
int nfc_close(nfc_driver_t *driver);

#include "iso14443.h"

#endif //NFC_H
