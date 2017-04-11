#ifndef TRF797X_H
#define TRF797X_H

#include <hal.h>

typedef enum {
    TRF797X_PROTO_ISO14443A,
    TRF797X_PROTO_ISO14443B,
    TRF797X_PROTO_ISO15693,
    TRF797X_PROTO_FELICA,
} trf797x_protocol_t;

typedef enum {
    TRF797XA_ST_IDLE,
    //TODO
    TRF797XA_ST_STOP,
} trf797x_state_t;


/** Error codes */
#define TRF797X_ERROR_CHECKSUM              -1
#define TRF797X_ERROR_PARITY                -2
#define TRF797X_ERROR_BYTE_FRAMING          -3
#define TRF797X_ERROR_COLLISION             -4
#define TRF797X_ERROR_TIMEOUT               -62

typedef struct {
    ioportid_t      port;
    ioportmask_t    pin;
} gpio_spec_t;

#define _trf79x_driver_data                                                 \
                event_source_t      event;                                  \
                trf797x_state_t     state;

#define _trf79x_config_data                                                 \
                struct {                                                    \
                        gpio_spec_t io[8];                                  \
                        gpio_spec_t en;                                     \
                        gpio_spec_t en2;                                    \
                        gpio_spec_t mod;                                    \
                    } gpio;                                                 \
                                                                            \
                    SPIDriver *spi;

typedef struct Trf797xDriver {
    _trf79x_driver_data
} Trf797xDriver;

typedef struct Trf797xConfig {
    _trf79x_config_data
} Trf797xConfig;

#include "trf797x_lld.h"
#include "trf797x_initiator.h"

/**
 * Initialize the driver structure.
 * @param driver
 */
void trf797x_init(Trf797xDriver *driver);

/**
 * Start the driver.
 * @param driver
 * @param config
 */
void trf797x_start(Trf797xDriver *driver, const Trf797xConfig *config);

/**
 * Interrupt hook.
 * @param driver
 */
void tf797x_interrupt_hookI(Trf797xDriver *driver);

/**
 * Stop the driver.
 * @param driver
 */
void trf797x_stop(Trf797xDriver *driver);


#endif //TRF797X_H