#ifndef TRF797X_INITIATOR_H
#define TRF797X_INITIATOR_H

#ifndef BIT
#define BIT(n)                              (1 << (n))
#endif

typedef struct Trf797xInitiatorConfig {
    _trf79x_config_data
    trf797x_protocol_t protocol;
} Trf797xInitiatorConfig;

typedef struct Trf797xInitiatorDriver {
    _trf79x_driver_data(Trf797xInitiatorConfig)
} Trf797xInitiatorDriver;

#define TRF797X_FLAG_TX_SDM                 BIT(0)  /**< use special direct mode for emission (raw mode) */

#define TRF797X_FLAG_RX_DM1                 BIT(0)  /**< use direct mode 1 for reception (raw mode) */
#define TRF797X_FLAG_RX_LSB_FIRST           BIT(1)  /**< if direct mode 1 is enabled, for reception */
#define TRF797X_FLAG_RX_IGNORE_CRC_ERROR    BIT(2)  /**< disable CRC verification */
#define TRF797X_FLAG_RX_IGNORE_PARITY_ERROR BIT(3)  /**< disable parity checks (ISO14443A) */

struct trf797x_transfer {
    void    *txbuf;
    size_t  txbits;
    int     txflags;

    void    *rxbuf;
    size_t  rxbits;     /**< input: size of rxbuf (in bits), output: number of bits received in rxbuf */
    int     rxflags;

    systime_t   timeout;
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
 * @param tr
 * @return zero on success, < 0 on error.
 */
int trf797x_initiator_transceive(Trf797xInitiatorDriver *driver, struct trf797x_transfer *tr);

/**
 * Stop the driver.
 * @param driver
 */
void trf797x_initiator_stop(Trf797xInitiatorDriver *driver);

#endif //TRF797X_INITIATOR_H
