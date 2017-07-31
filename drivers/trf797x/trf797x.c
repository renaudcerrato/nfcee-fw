#include "trf797x.h"
#include "trf797x_lld.h"

#define gpio_set(gpio) do{ gpioSetPad(gpio); }while(0)
#define gpio_clr(gpio) do{ gpioClearPad(gpio); }while(0)

/**
 * Power-up sequence.
 */
static void reset_hardware(const Trf797xConfig *cfg) {
    gpio_clr(cfg->gpio.en);
    gpio_clr(cfg->gpio.io[0]); // SPI with SS
    gpio_set(cfg->gpio.io[1]); // SPI with SS
    gpio_set(cfg->gpio.io[2]); // SPI with SS

    spiUnselect(cfg->spi);
    chThdSleepMilliseconds(2);
    spiSelect(cfg->spi);
    chThdSleepMilliseconds(4);
    gpio_set(cfg->gpio.en);
    chThdSleepMilliseconds(5);  // how much?
}

static bool trf797x_initialize(const Trf797xConfig *cfg) {

    uint8_t mod_sys_clk = TRF7970X_MODULATOR_DEPTH_OOK;

#if TRF797X_CONF_SYS_CLK_DIV == 0
    mod_sys_clk|=TRF7970X_MODULATOR_CLK(TRF7970X_SYS_CLK_DISABLED);
#elif TRF797X_CONF_SYS_CLK_DIV == 1
    mod_sys_clk|=TRF7970X_MODULATOR_CLK(TRF7970X_SYS_CLK_DIV1);
#elif TRF797X_CONF_SYS_CLK_DIV == 2
    mod_sys_clk|=TRF7970X_MODULATOR_CLK(TRF7970X_SYS_CLK_DIV2);
#elif TRF797X_CONF_SYS_CLK_DIV == 4
    mod_sys_clk|=TRF7970X_MODULATOR_CLK(TRF7970X_SYS_CLK_DIV4);
#else
    #error "invalid value: TRF797X_CONF_SYS_CLK_DIV"
#endif

    trf797x_command(cfg->spi, TRF797X_CMD_INIT);
    trf797x_command(cfg->spi, TRF797X_CMD_IDLE);
    chThdSleepMilliseconds(1);

    trf797x_command(cfg->spi, TRF797X_CMD_RESET_FIFO);

#if TRF797X_CONF_27MHZ_OSC == 1
    mod_sys_clk |= TRF7970X_MODULATOR_27MHZ;
#endif

    trf797x_register_write1(cfg->spi, TRF797X_REG_MODULATOR_SYS_CLK, mod_sys_clk);

    trf797x_register_write1(cfg->spi, TRF797X_REG_NFC_TARGET_DETECTION, 0);  // see errata

    trf797x_register_write1(cfg->spi, TRF797X_REG_CHIP_STATUS,
                            (TRF797X_CONF_VIN_5V) ? TRF797X_CHIP_STATUS_VRS5_3 : 0);

    return trf797x_register_read1(cfg->spi, TRF797X_REG_MODULATOR_SYS_CLK) == mod_sys_clk;
}

void trf797x_driver_init(Trf797xDriver *driver) {
    driver->state = TRF797X_ST_PWR_OFF;
    driver->config = NULL;
    chEvtObjectInit(&driver->event);
}

int trf797x_start(Trf797xDriver *drv, const Trf797xConfig *config) {

    if(drv->state == TRF797X_ST_PWR_OFF) {
        reset_hardware(config);
    }

    const bool found = trf797x_initialize(config);
    if(!found) {
        return TRF797X_ERR_PROBE;
    }

    drv->state = TRF797X_ST_IDLE;
    drv->config = config;

    chEvtObjectInit(&drv->event);
    chEvtRegister(&drv->event, &drv->listener, config->event);

    return 0;
}

void tf797x_interrupt_hookI(Trf797xDriver *drv) {
    chEvtBroadcastFlagsI(&drv->event, EVENT_IRQ);
}

int trf797x_switch_rf(Trf797xDriver *driver, bool on) {
    //TODO: check RF level
    trf797x_register_write1(driver->config->spi, TRF797X_REG_CHIP_STATUS,
                                    ((on) ? TRF797X_CHIP_STATUS_RF_ON : 0) |
                                    ((TRF797X_CONF_VIN_5V) ? TRF797X_CHIP_STATUS_VRS5_3 : 0));
    return 0;
}

void trf797x_stop(Trf797xDriver *drv, bool shutdown) {

    if(shutdown) {
        gpio_clr(drv->config->gpio.en);
        drv->state = TRF797X_ST_PWR_OFF;
    }

    chEvtBroadcastFlags(&drv->event, EVENT_STOP);
}
