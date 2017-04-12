#include "trf797x.h"
#include "trf797x_lld.h"
#include "trf797x_conf.h"

#define EVENT_IRQ               (1 << 0)
#define EVENT_STOP              (1 << 31)

/**
 * Power-up sequence.
 */
static void gpio_power_up(void) {
    CLEAR(EN);

#if TRF797X_USE_EN2
    CLEAR(EN2);
#endif

#if TRF797X_USE_IO0
    CLEAR(IO0);     // SPI with SS
#endif

#if TRF797X_USE_IO1
    SET(IO1);       // SPI with SS
#endif

#if TRF797X_USE_IO2
    SET(IO2);       // SPI with SS
#endif

    CLEAR(CS);
    chThdSleepMilliseconds(2);
    SET(CS);
    chThdSleepMilliseconds(3);
    SET(EN);
    chThdSleepMilliseconds(5); // how much?
}

void trf797x_driver_init(Trf797xDriver *driver) {
    driver->state = TRF797XA_ST_IDLE;
    osalEventObjectInit(&driver->event);
}

void trf797x_shutdown(Trf797xDriver *driver) {
    CLEAR(EN);
#if TRF797X_USE_EN2
    CLEAR(EN2);
#endif
    driver->state = TRF797XA_ST_SHUTDOWN;
}

bool trf797x_power_up(Trf797xDriver *drv) {

    gpio_power_up();

    trf797x_acquire_bus(drv->config->spi);
    trf797x_command(drv->config->spi, TRF797X_CMD_INIT);
    trf797x_command(drv->config->spi, TRF797X_CMD_IDLE);
    chThdSleepMilliseconds(1);

    trf797x_command(drv->config->spi, TRF797X_CMD_RESET_FIFO);

    uint8_t mod_sys_clk = TRF7970X_MODULATOR_DEPTH_OOK;

#if TRF797X_CFG_OSCILLATOR_27MHZ
    mod_sys_clk|=TRF7970X_MODULATOR_27MHZ;
#endif

#if TRF797X_CFG_SYS_CLK_DIVIDER == 1
    mod_sys_clk|=TRF7970X_SYS_CLK_DIV1;
#elif TRF797X_CFG_SYS_CLK_DIVIDER == 2
    mod_sys_clk|=TRF7970X_SYS_CLK_DIV2;
#elif TRF797X_CFG_SYS_CLK_DIVIDER == 4
    mod_sys_clk|=TRF7970X_SYS_CLK_DIV4;
#elif TRF797X_CFG_SYS_CLK_DIVIDER == 0
    mod_sys_clk|=TRF7970X_SYS_CLK_DISABLED;
#endif

    trf797x_register_write1(drv->config->spi, TRF797X_REG_MODULATOR_SYS_CLK, mod_sys_clk);
    trf797x_register_write1(drv->config->spi, TRF797X_REG_NFC_TARGET_DETECTION, 0);  // see errata

    const bool found = trf797x_register_read1(drv->config->spi, TRF797X_REG_MODULATOR_SYS_CLK) == mod_sys_clk;

    trf797x_release_bus(drv->config->spi);

    return found;
}

void tf797x_interrupt_hookI(Trf797xDriver *driver) {
    osalSysLockFromISR();
    osalEventBroadcastFlagsI(&driver->event, EVENT_IRQ);
    osalSysUnlockFromISR();
}

void trf797x_stop(Trf797xDriver *driver) {
    driver->state = TRF797XA_ST_STOP;
    osalEventBroadcastFlags(&driver->event, EVENT_STOP);
}