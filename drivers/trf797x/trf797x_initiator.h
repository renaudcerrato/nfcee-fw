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

#define TRF797X_FLAG_TX_SDM                 BIT(0)  /**< use special direct mode for emission (raw mode) */

#define TRF797X_FLAG_RX_DM1                 BIT(0)  /**< use direct mode 1 for reception (raw mode) */
#define TRF797X_FLAG_RX_LSB_FIRST           BIT(1)  /**< valid only if TRF797X_FLAG_RX_DM1 is set */
#define TRF797X_FLAG_RX_IGNORE_CRC_ERROR    BIT(2)  /**< disable CRC verification */
#define TRF797X_FLAG_RX_IGNORE_PARITY_ERROR BIT(3)  /**< disable parity checks (ISO14443A) */

struct trf797x_transfer {
    const void      *txbuf;
    size_t          txbits;
    int             txflags;

    void            *rxbuf;
    size_t          rxbits;     /**< input: size of rxbuf (in bits), output: number of bits received in rxbuf */
    int             rxflags;

    systime_t       timeout;
};

/**
 * Initialize the driver structure.
 * @param driver
 */
void trf797x_initiator_driver_init(Trf797xInitiatorDriver *driver);

/**
 * Start the driver.
 * @param drv
 * @param config
 */
int trf797x_initiator_start(Trf797xInitiatorDriver *drv, const Trf797xInitiatorConfig *config);


/**
 * Transceive data to/from the device.
 * @param driver
 * @param tr
 * @return zero on success, < 0 on error.
 */
int trf797x_initiator_transceive(Trf797xInitiatorDriver *driver, struct trf797x_transfer *tr);

/**
 * Stop the driver.
 * @param drv
 */
void trf797x_initiator_stop(Trf797xInitiatorDriver *drv, bool shutdown);

#endif //TRF797X_INITIATOR_H
