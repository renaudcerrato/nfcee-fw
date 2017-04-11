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
#define TRF797X_ERROR_PROBE                 -1000

typedef struct {
    ioportid_t      port;
    ioportmask_t    pin;
} gpio_spec_t;

#define _trf79x_driver_data(CONFIG)                                         \
                event_source_t      event;                                  \
                trf797x_state_t     state;                                  \
                const CONFIG        *config;

#define _trf79x_config_data                                                 \
                SPIDriver *spi;

typedef struct Trf797xConfig {
    _trf79x_config_data
} Trf797xConfig;

typedef struct Trf797xDriver {
    _trf79x_driver_data(Trf797xConfig)
} Trf797xDriver;


#include "trf797x_lld.h"
#include "trf797x_initiator.h"

/**
 * Initialize the driver object.
 * @param driver
 */
void trf797x_driver_init(Trf797xDriver *driver);

/**
 * Configure and start the driver.
 * @param driver
 * @param config
 * @return zero on success, < 0 otherwise.
 */
int trf797x_start(Trf797xDriver *driver, const Trf797xConfig *config);

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