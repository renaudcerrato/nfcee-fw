#ifndef TRF797X_H
#define TRF797X_H

#include <hal.h>
#include <errno.h>
#include "trfconf.h"

#ifndef TRF797X_CONF_27MHZ_OSC
#error "you must define TRF797X_CONF_27MHZ_OSC"
#endif

#ifndef TRF797X_CONF_VIN_5V
#error "you must define TRF797X_CONF_VIN_5V"
#endif

#ifndef TRF797X_CONF_SYS_CLK_DIV
#define TRF797X_CONF_SYS_CLK_DIV                0
#endif

#define EVENT_STOP              (1 << 0)
#define EVENT_IRQ               (1 << 1)

enum trf797x_state {
    TRF797X_ST_PWR_OFF,
    TRF797X_ST_RF_OFF,
    TRF797X_ST_IDLE,
};

/** Error codes */
enum {
    TRF797X_ERR_BASE = __ELASTERROR,
    TRF797X_ERR_CHECKSUM,
    TRF797X_ERR_PARITY,
    TRF797X_ERR_FRAMING,
    TRF797X_ERR_COLLISION,
};

#define _trf79x_driver_data(CONFIG)                                         \
                event_source_t      event;                                  \
                event_listener_t    listener;                               \
                enum trf797x_state  state;                                  \
                const CONFIG        *config;

#define _trf79x_config_data                                                 \
                SPIDriver                       *spi;                       \
                eventid_t                       event;                      \
                struct {                                                    \
                    gpio_t     en;                                          \
                    gpio_t     mod;                                         \
                    gpio_t     io[8];                                       \
                } gpio;

typedef struct Trf797xConfig {
    _trf79x_config_data
} Trf797xConfig;

typedef struct Trf797xDriver {
    _trf79x_driver_data(Trf797xConfig)
} Trf797xDriver;

struct trf797x_iovec {
    void                *base;
    union {
        uint16_t        bits;
        struct {
#ifndef __BYTE_ORDER__
#error "you must define __BYTE_ORDER__"
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            uint16_t    :3;
            uint16_t    bytes:14;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            uint16_t    bytes:14;
            uint16_t    :3;
#else
#error "unsupported endian"
#endif
        };
    };
};

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
 * Switch RF on/off
 * @param driver
 * @param on
 * @return
 */
int trf797x_switch_rf(Trf797xDriver *driver, bool on);

/**
 * Stop the driver.
 * @param driver
 */
void trf797x_stop(Trf797xDriver *driver, bool shutdown);


#endif //TRF797X_H