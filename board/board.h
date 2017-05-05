#ifndef _BOARD_H_
#define _BOARD_H_

#include "gpio.h"

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

/*
 * TRACE definition
 */
#define TRACE_SD					SD1

/*
 * GPIO definitions
 */
#define GPIO_UART_TX                GPIO_A(9)
#define GPIO_UART_TX_MODE           GPIO_MODE_ALTERNATE(7) | GPIO_STM32_OSPEED_HIGHEST

#define GPIO_UART_RX                GPIO_A(10)
#define GPIO_UART_RX_MODE           GPIO_MODE_ALTERNATE(7)

#define GPIO_OTG_FS_DM              GPIO_A(11)
#define GPIO_OTG_FS_DM_MODE         GPIO_MODE_ALTERNATE(10) | GPIO_STM32_OSPEED_HIGHEST

#define GPIO_OTG_FS_DP              GPIO_A(12)
#define GPIO_OTG_FS_DP_MODE         GPIO_MODE_ALTERNATE(10) | GPIO_STM32_OSPEED_HIGHEST

#define GPIO_LED0                   GPIO_B(6)
#define GPIO_LED0_MODE              GPIO_MODE_OUTPUT_PUSHPULL

#define GPIO_LED1                   GPIO_B(7)
#define GPIO_LED1_MODE              GPIO_MODE_OUTPUT_PUSHPULL

#define GPIO_BUTTON                 GPIO_B(8)
#define GPIO_BUTTON_MODE            GPIO_MODE_INPUT

#define GPIO_MOD                    GPIO_A(1)
#define GPIO_MOD_MODE               GPIO_MODE_INPUT_PULLUP

#define GPIO_SPI1_SCK               GPIO_A(5)
#define GPIO_SPI1_SCK_MODE          GPIO_MODE_ALTERNATE(5) | GPIO_STM32_OSPEED_HIGHEST

#define GPIO_EN2                    GPIO_B(13)
#define GPIO_EN2_MODE               GPIO_MODE_OUTPUT_PUSHPULL

#define GPIO_EN                     GPIO_B(14)
#define GPIO_EN_MODE                GPIO_MODE_OUTPUT_PUSHPULL

#define GPIO_ASK_OOK                GPIO_B(15)
#define GPIO_ASK_OOK_MODE           GPIO_MODE_OUTPUT_PUSHPULL

#define GPIO_IRQ                    GPIO_C(6)
#define GPIO_IRQ_MODE               GPIO_MODE_INPUT_PULLUP

#define GPIO_IO0                    GPIO_C(11)
#define GPIO_IO0_MODE               GPIO_MODE_OUTPUT_PUSHPULL

#define GPIO_IO1                    GPIO_C(10)
#define GPIO_IO1_MODE               GPIO_MODE_OUTPUT_PUSHPULL

#define GPIO_IO2                    GPIO_C(9)
#define GPIO_IO2_MODE               GPIO_MODE_OUTPUT_PUSHPULL

#define GPIO_IO3                    GPIO_A(8)
#define GPIO_IO3_MODE               GPIO_MODE_INPUT_PULLUP

#define GPIO_IO4                    GPIO_A(4)
#define GPIO_IO4_MODE               GPIO_MODE_OUTPUT_PUSHPULL | GPIO_STM32_OSPEED_HIGHEST

#define GPIO_IO5                    GPIO_B(12)
#define GPIO_IO5_MODE               GPIO_MODE_INPUT_PULLUP

#define GPIO_IO6                    GPIO_B(4)       // SPI1_MISO
#define GPIO_IO6_MODE               GPIO_MODE_ALTERNATE(5)

#define GPIO_IO7                    GPIO_B(5)       // SPI1_MOSI
#define GPIO_IO7_MODE               GPIO_MODE_ALTERNATE(5) | GPIO_STM32_OSPEED_HIGHEST

#define GPIO_CS                     GPIO_IO4


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