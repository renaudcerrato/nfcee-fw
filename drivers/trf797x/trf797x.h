#ifndef TRF797X_H
#define TRF797X_H

#include <hal.h>

#define EVENT_STOP              (1 << 0)
#define EVENT_IRQ               (1 << 1)

enum trf797x_state {
    TRF797X_ST_PWR_OFF,
    TRF797X_ST_RF_OFF,
    TRF797X_ST_IDLE,
    TRF797X_ST_WAIT_FOR_TX,
    TRF797X_ST_WAIT_FOR_RX,
    TRF797X_ST_STOP,
};

/** Error codes */
#define TRF797X_ERR_TIMEOUT               -62
#define TRF797X_ERR_PROBE                 -100
#define TRF797X_ERR_CHECKSUM              -1001
#define TRF797X_ERR_PARITY                -1002
#define TRF797X_ERR_FRAMING               -1003
#define TRF797X_ERR_COLLISION             -1004


typedef struct {
    ioportid_t      port;
    ioportmask_t    pin;
} trf_gpio_spec_t;

enum trf797x_sys_clk_divider {
    TRF7970X_SYS_CLK_DISABLED = 0,
    TRF7970X_SYS_CLK_DIV1,
    TRF7970X_SYS_CLK_DIV2,
    TRF7970X_SYS_CLK_DIV4,
};

enum trf797x_vin_voltage {
    TRF7970X_VIN_5V,
    TRF7970X_VIN_3V,
};

#define _trf79x_driver_data(CONFIG)                                         \
                event_source_t      event;                                  \
                event_listener_t    listener;                               \
                enum trf797x_state  state;                                  \
                const CONFIG        *config;

#define _trf79x_config_data                                                 \
                SPIDriver                       *spi;                       \
                int                             event;                      \
                bool                            osc27m;                     \
                enum trf797x_sys_clk_divider    div;                        \
                enum trf797x_vin_voltage        vin;                        \
                struct {                                                    \
                    trf_gpio_spec_t     en;                                 \
                    trf_gpio_spec_t     mod;                                \
                    trf_gpio_spec_t     io[8];                              \
                } gpio;

typedef struct Trf797xConfig {
    _trf79x_config_data
} Trf797xConfig;

typedef struct Trf797xDriver {
    _trf79x_driver_data(Trf797xConfig)
} Trf797xDriver;


#include "trf797x_initiator.h"

/**
 * Initialize the driver object.
 * @param driver
 */
void trf797x_driver_init(Trf797xDriver *driver);

/**
 * Configure and start the driver.
 * @param drv
 * @param config
 * @return zero on success, < 0 otherwise.
 */
int trf797x_start(Trf797xDriver *drv, const Trf797xConfig *config);

/**
 * Interrupt hook.
 * @param driver
 */
void tf797x_interrupt_hookI(Trf797xDriver *driver);

/**
 * Stop the driver.
 * @param driver
 */
void trf797x_stop(Trf797xDriver *driver, bool shutdown);


#endif //TRF797X_H