#include "trf797x.h"
#include "trf797x_lld.h"


#define gpio_set(spec) do{ palSetPad(spec.port, spec.pin); }while(0)
#define gpio_clr(spec) do{ palClearPad(spec.port, spec.pin); }while(0)


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
    spiSelect(cfg->spi);        // is it really necessary?
    chThdSleepMilliseconds(3);
    gpio_set(cfg->gpio.en);
    chThdSleepMilliseconds(5);  // how much?
    spiUnselect(cfg->spi);
}

static bool trf797x_initialize(const Trf797xConfig *cfg) {

    uint8_t mod_sys_clk = TRF7970X_MODULATOR_DEPTH_OOK | TRF7970X_MODULATOR_CLK(cfg->div);

    trf797x_command(cfg->spi, TRF797X_CMD_INIT);
    trf797x_command(cfg->spi, TRF797X_CMD_IDLE);
    chThdSleepMilliseconds(1);

    trf797x_command(cfg->spi, TRF797X_CMD_RESET_FIFO);

    if(cfg->osc27m) {
        mod_sys_clk |= TRF7970X_MODULATOR_27MHZ;
    }

    trf797x_register_write1(cfg->spi, TRF797X_REG_MODULATOR_SYS_CLK, mod_sys_clk);
    trf797x_register_write1(cfg->spi, TRF797X_REG_NFC_TARGET_DETECTION, 0);  // see errata

    const bool found = trf797x_register_read1(cfg->spi, TRF797X_REG_MODULATOR_SYS_CLK) == mod_sys_clk;

    return found;
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

    drv->config = config;
    chEvtRegisterMask(&drv->event, &drv->listener, EVENT_MASK(config->event));

    return 0;
}

void tf797x_interrupt_hookI(Trf797xDriver *drv) {
    chSysLockFromISR();
    chEvtBroadcastFlagsI(&drv->event, EVENT_IRQ);
    chSysUnlockFromISR();
}

void trf797x_stop(Trf797xDriver *drv, bool shutdown) {

    if(shutdown) {
        gpio_clr(drv->config->gpio.en);
        drv->state = TRF797X_ST_PWR_OFF;
    }

    chEvtBroadcastFlags(&drv->event, EVENT_STOP);
}