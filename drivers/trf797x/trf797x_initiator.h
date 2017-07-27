#ifndef TRF797X_INITIATOR_H
#define TRF797X_INITIATOR_H

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

struct trf797x_tx {
    void                *buf;
    union {
        uint32_t bits;
        struct {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            uint32_t :3;
            uint32_t bytes:29;
#else
            uint32_t bytes:29;
            uint32_t :3;
#endif
        };
    };
};

struct trf797x_rx {
    void                *buf;
    uint32_t            bytes;
    systime_t           timeout;
};

/**
 * Initialize the driver structure.
 * @param driver
 */
void trf797x_initiator_driver_init(Trf797xInitiatorDriver *driver);

/**
 * Start the driver.
 * @param driver
 * @param config
 */
int trf797x_initiator_start(Trf797xInitiatorDriver *driver, const Trf797xInitiatorConfig *config);


/**
 * Transceive data to/from the device.
 * @param driver
 * @param tx
 * @param rx
 * @return the # of bytes received in rx->buf, or < 0 on error.
 */
int trf797x_initiator_transceive(Trf797xInitiatorDriver *drv, const struct trf797x_tx *tx, const struct trf797x_rx *rx);

/**
 * Stop the driver.
 * @param driver
 * @param shutdown
 */
void trf797x_initiator_stop(Trf797xInitiatorDriver *driver, bool shutdown);

#endif //TRF797X_INITIATOR_H
