#ifndef ISO14443_H
#define ISO14443_H

#ifndef NFC_H
#error "you must not include this file directly, include <nfc.h> instead."
#endif

typedef uint8_t     iso14443a_sak_t;
typedef uint16_t    iso14443a_atqa_t;

struct nfcid {
    uint8_t len;
    uint8_t data[10];
} __attribute__((packed));

#define _nfc_driver_iso14443_data       _nfc_driver_data                        \
                                        struct nfc_iso14443_fops    l3;         \
                                        struct nfcid                nfcid;      \
                                        size_t                      fsc,fsd;    \
                                        unsigned int                fwt;        \
                                        uint32_t                    block;

typedef struct nfc_iso14443_driver nfc_iso14443_driver_t;

struct nfc_iso14443_fops {
    int (*open)(nfc_iso14443_driver_t *driver);
    int (*transceive)(nfc_iso14443_driver_t *driver, const void *tx, size_t txlen, void *rx, size_t rxlen, unsigned int timeout);
    int (*close)(nfc_iso14443_driver_t *driver);
};

typedef struct nfc_iso14443_driver {
    _nfc_driver_iso14443_data
} nfc_iso14443_driver_t;

typedef struct {
    _nfc_driver_iso14443_data
    iso14443a_sak_t              sak;
    iso14443a_atqa_t             atqa;
} nfc_iso14443a_driver_t;

typedef struct {
    _nfc_driver_iso14443_data
    uint8_t                     atqb[12];
} nfc_iso14443b_driver_t;

void nfc_iso14443a_driver_init(nfc_iso14443a_driver_t *driver, struct nfc_device *dev);

int nfc_iso14443_open(nfc_iso14443_driver_t *driver);
int nfc_iso14443_transceive(nfc_iso14443_driver_t *driver, const void *tx, size_t txlen, void *rx, size_t rxlen, unsigned int timeout);
int nfc_iso14443_close(nfc_iso14443_driver_t *driver);

#endif //ISO14443_H
