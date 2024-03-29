#include <hal.h>
#include "trf797x.h"

#define CONFIGURE_PAD(NAME)    gpioSetPadMode(NAME, NAME ## _MODE)


void trf797x_extcallback(EXTDriver *extp, expchannel_t channel);

static const SPIConfig trf797x_spicfg = {
        NULL,
        GPIO_PORT(GPIO_IO4),
        GPIO_PAD(GPIO_IO4),
        SPI_CR1_CPHA |
#if (STM32_PCLK1 / 2) <= TRF797X_CONF_SPI_CLOCK
        0
#elif (STM32_PCLK1 / 4) <= TRF797X_CONF_SPI_CLOCK
        SPI_CR1_BR_0
#elif (STM32_PCLK1 / 8) <= TRF797X_CONF_SPI_CLOCK
        SPI_CR1_BR_1
#elif (STM32_PCLK1 / 16) <= TRF797X_CONF_SPI_CLOCK
        SPI_CR1_BR_1 | SPI_CR1_BR_0
#elif (STM32_PCLK1 / 32) <= TRF797X_CONF_SPI_CLOCK
        SPI_CR1_BR_2
#elif (STM32_PCLK1 / 64) <= TRF797X_CONF_SPI_CLOCK
        SPI_CR1_BR_2 | SPI_CR1_BR_0
#elif (STM32_PCLK1 / 128) <= TRF797X_CONF_SPI_CLOCK
        SPI_CR1_BR_2 | SPI_CR1_BR_1
#elif (STM32_PCLK1 / 256) <= TRF797X_CONF_SPI_CLOCK
        SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0
#else
    #error "unable to compute SPI baudrate for given TRF797X_CONF_SPI_CLOCK"
#endif
};

static const EXTConfig extcfg = {
    {
        [TRF797X_EXT_CHANNEL] = {EXT_CH_MODE_RISING_EDGE | TRF797X_EXT_MODE_GPIO, &trf797x_extcallback},
    }
};

void __early_init(void) {

    stm32_clock_init();
}

/**
 * @brief   Board-specific initialization code.
 */
void boardInit(void) {

    gpioInit();

    // TRF7970A
    gpioClearPad(GPIO_EN);
    gpioClearPad(GPIO_EN2);
    gpioSetPad(GPIO_IO4);       // unselect

    CONFIGURE_PAD(GPIO_EN);
    CONFIGURE_PAD(GPIO_EN2);
    CONFIGURE_PAD(GPIO_IRQ);
    CONFIGURE_PAD(GPIO_MOD);
    CONFIGURE_PAD(GPIO_ASK_OOK);

    CONFIGURE_PAD(GPIO_IO0);
    CONFIGURE_PAD(GPIO_IO1);
    CONFIGURE_PAD(GPIO_IO2);
    CONFIGURE_PAD(GPIO_IO3);
    CONFIGURE_PAD(GPIO_IO4);
    CONFIGURE_PAD(GPIO_IO5);
    CONFIGURE_PAD(GPIO_IO6);
    CONFIGURE_PAD(GPIO_IO7);

    // SPI1
    CONFIGURE_PAD(GPIO_SPI1_SCK);

    // UART
    CONFIGURE_PAD(GPIO_UART_TX);
    CONFIGURE_PAD(GPIO_UART_RX);

    // USB
    CONFIGURE_PAD(GPIO_OTG_FS_DM);
    CONFIGURE_PAD(GPIO_OTG_FS_DP);

    // MISC
    CONFIGURE_PAD(GPIO_LED0);
    CONFIGURE_PAD(GPIO_LED1);
    CONFIGURE_PAD(GPIO_BUTTON);

    // Configure SPI
    spiStart(&SPID1, &trf797x_spicfg);

    // Configure EXT driver
    extStart(&EXTD1, &extcfg);
}