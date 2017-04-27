
#include "hal.h"

/**
 * @brief   Early initialization code.
 * @details This initialization must be performed just after stack setup
 *          and before any other initialization.
 */
void __early_init(void) {

  stm32_clock_init();
}


/**
 * @brief   Board-specific initialization code.
 */
void boardInit(void) {

  gpioInit();

  gpioSetPadMode(GPIO_MOD, GPIO_MODE_INPUT_PULLUP);
  gpioSetPadMode(GPIO_DM_DATA_OUT, GPIO_MODE_INPUT_PULLUP);
  gpioSetPadMode(GPIO_SPI1_SCK, GPIO_MODE_ALTERNATE(5));
  gpioSetPadMode(GPIO_SDM_TX, GPIO_MODE_INPUT_PULLUP);
  gpioSetPadMode(GPIO_UART_TX, GPIO_MODE_ALTERNATE(7));
  gpioSetPadMode(GPIO_UART_RX, GPIO_MODE_ALTERNATE(7));
  gpioSetPadMode(GPIO_OTG_FS_DM, GPIO_MODE_ALTERNATE(10));
  gpioSetPadMode(GPIO_OTG_FS_DP, GPIO_MODE_ALTERNATE(10));
  gpioSetPadMode(GPIO_SWDIO, GPIO_MODE_ALTERNATE(0));
  gpioSetPadMode(GPIO_SWCLK, GPIO_MODE_ALTERNATE(0));

  gpioSetPadMode(GPIO_SPI3_SCK, GPIO_MODE_ALTERNATE(6));
  gpioSetPadMode(GPIO_SPI_MISO, GPIO_MODE_ALTERNATE(5));
  gpioSetPadMode(GPIO_SPI_MOSI, GPIO_MODE_ALTERNATE(5));
  gpioSetPadMode(GPIO_LED0, GPIO_MODE_OUTPUT_PUSHPULL);
  gpioSetPadMode(GPIO_LED1, GPIO_MODE_OUTPUT_PUSHPULL);
  gpioSetPadMode(GPIO_BUTTON, GPIO_MODE_INPUT);
  gpioSetPadMode(GPIO_DM_CLK, GPIO_MODE_INPUT_PULLUP);
  gpioSetPadMode(GPIO_EN2, GPIO_MODE_OUTPUT_PUSHPULL);
  gpioSetPadMode(GPIO_EN, GPIO_MODE_OUTPUT_PUSHPULL);
  gpioSetPadMode(GPIO_ASK_OOK, GPIO_MODE_OUTPUT_PUSHPULL);

  gpioSetPadMode(GPIO_IRQ, GPIO_MODE_INPUT_PULLUP);
  gpioSetPadMode(GPIO_IO0, GPIO_MODE_OUTPUT_PUSHPULL);
  gpioSetPadMode(GPIO_IO1, GPIO_MODE_OUTPUT_PUSHPULL);
  gpioSetPadMode(GPIO_SDM_TX_EN, GPIO_MODE_OUTPUT_PUSHPULL);
  gpioSetPadMode(GPIO_CS, GPIO_MODE_OUTPUT_PUSHPULL);
  gpioSetPadMode(GPIO_SPI3_MOSI, GPIO_MODE_ALTERNATE(6));

  gpioSetPadMode(GPIO_OSC_IN, GPIO_MODE_INPUT | GPIO_STM32_PUPDR_FLOATING);
  gpioSetPadMode(GPIO_OSC_OUT, GPIO_MODE_INPUT | GPIO_STM32_PUPDR_FLOATING);
}
