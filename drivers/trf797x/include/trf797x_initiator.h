#ifndef TRF797X_INITIATOR_H
#define TRF797X_INITIATOR_H

#ifndef TRF797X_H
#error "you must not include this file directly, include <trf797x.h> instead."
#endif

#ifndef BIT
#define BIT(n)                              (1 << (n))
#endif

enum trf797x_protocol_rfid {
    TRF7970X_PROTO_RFID_15693_SGL_1OF4_662    = 0x00,
    TRF7970X_PROTO_RFID_15693_SGL_1OF256_662  = 0x01,
    TRF7970X_PROTO_RFID_15693_SGL_1OF4_2648   = 0x02,
    TRF7970X_PROTO_RFID_15693_SGL_1OF256_2648 = 0x03,
    TRF7970X_PROTO_RFID_15693_DBL_1OF4_667a   = 0x04,
    TRF7970X_PROTO_RFID_15693_DBL_1OF256_667  = 0x05,
    TRF7970X_PROTO_RFID_15693_DBL_1OF4_2669   = 0x06,
    TRF7970X_PROTO_RFID_15693_DBL_1OF256_2669 = 0x07,
    TRF7970X_PROTO_RFID_14443A_106            = 0x08,
    TRF7970X_PROTO_RFID_14443A_212            = 0x09,
    TRF7970X_PROTO_RFID_14443A_424            = 0x0a,
    TRF7970X_PROTO_RFID_14443A_848            = 0x0b,
    TRF7970X_PROTO_RFID_14443B_106            = 0x0c,
    TRF7970X_PROTO_RFID_14443B_212            = 0x0d,
    TRF7970X_PROTO_RFID_14443B_424            = 0x0e,
    TRF7970X_PROTO_RFID_14443B_848            = 0x0f,
    TRF7970X_PROTO_RFID_FELICA_212            = 0x1a,
    TRF7970X_PROTO_RFID_FELICA_424            = 0x1b,
};

typedef struct Trf797xInitiatorConfig {
    _trf79x_config_data
    enum trf797x_protocol_rfid proto;
} Trf797xInitiatorConfig;

typedef struct Trf797xInitiatorDriver {
    _trf79x_driver_data(Trf797xInitiatorConfig)
} Trf797xInitiatorDriver;


void trf797x_initiator_driver_init(Trf797xInitiatorDriver *driver);
int trf797x_initiator_start(Trf797xInitiatorDriver *driver, const Trf797xInitiatorConfig *config);
int trf797x_initiator_transceive(Trf797xInitiatorDriver *drv, const struct trf797x_iovec *tx, size_t len, const struct trf797x_iovec *rx, systime_t timeout);
int trf797x_initiator_stop(Trf797xInitiatorDriver *driver, bool shutdown);

#if TRF797X_CONF_HAVE_LIBNFC
#include <nfc.h>

struct trf797x_nfc_device {
    _nfc_device_data
    Trf797xInitiatorDriver  driver;
    Trf797xInitiatorConfig  config;
};

void trf797x_nfc_device_init(struct trf797x_nfc_device *device);
#endif

#endif //TRF797X_INITIATOR_H
