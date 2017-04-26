#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0U
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   330U

//###############################################################


#define GPIO_MOD                    GPIO_A(1, PAL_MODE_INPUT_PULLUP)
#define GPIO_DM_DATA_OUT            GPIO_A(2, PAL_MODE_INPUT_PULLUP)
#define GPIO_SPI1_SCK               GPIO_A(5, PAL_MODE_ALTERNATE(5))
#define GPIO_SDM_TX                 GPIO_A(8, PAL_MODE_INPUT_PULLUP)
#define GPIO_UART_TX                GPIO_A(9, PAL_MODE_ALTERNATE(7))
#define GPIO_UART_RX                GPIO_A(10, PAL_MODE_ALTERNATE(7))
#define GPIO_OTG_FS_DM              GPIO_A(11, PAL_MODE_ALTERNATE(10))
#define GPIO_OTG_FS_DP              GPIO_A(12, PAL_MODE_ALTERNATE(10))
#define GPIO_SWDIO                  GPIO_A(13, PAL_MODE_ALTERNATE(0))
#define GPIO_SWCLK                  GPIO_A(14, PAL_MODE_ALTERNATE(0))

#define GPIO_SPI3_SCK               GPIO_B(3, PAL_MODE_ALTERNATE(6))
#define GPIO_SPI_MISO               GPIO_B(4, PAL_MODE_ALTERNATE(5))
#define GPIO_SPI_MOSI               GPIO_B(5, PAL_MODE_ALTERNATE(5))
#define GPIO_LED0                   GPIO_B(6, PAL_MODE_OUTPUT_PUSHPULL)
#define GPIO_LED1                   GPIO_B(7, PAL_MODE_OUTPUT_PUSHPULL)
#define GPIO_BUTTON                 GPIO_B(8, PAL_MODE_INPUT)
#define GPIO_DM_CLK                 GPIO_B(12, PAL_MODE_INPUT_PULLUP)
#define GPIO_EN2                    GPIO_B(13, PAL_MODE_OUTPUT_PUSHPULL)
#define GPIO_EN                     GPIO_B(14, PAL_MODE_OUTPUT_PUSHPULL)
#define GPIO_ASK_OOK                GPIO_B(15, PAL_MODE_OUTPUT_PUSHPULL)

#define GPIO_IRQ                    GPIO_C((6, PAL_MODE_INPUT_PULLUP)
#define GPIO_IO0                    GPIO_C((7, PAL_MODE_OUTPUT_PUSHPULL)
#define GPIO_IO1                    GPIO_C((8, PAL_MODE_OUTPUT_PUSHPULL)
#define GPIO_SDM_TX_EN              GPIO_C((9, PAL_MODE_OUTPUT_PUSHPULL)
#define GPIO_CS                     GPIO_C((10, PAL_MODE_OUTPUT_PUSHPULL)
#define GPIO_SPI3_MOSI              GPIO_C((12, PAL_MODE_ALTERNATE(6))

#define GPIO_OSC_IN                 GPIO_H(0, PAL_MODE_INPUT | PAL_STM32_PUPDR_FLOATING)
#define GPIO_OSC_OUT                GPIO_H(1, PAL_MODE_INPUT | PAL_STM32_PUPDR_FLOATING)


//###############################################################


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
