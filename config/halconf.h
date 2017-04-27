#ifndef _HALCONF_H_
#define _HALCONF_H_

#include "mcuconf.h"

/**
 * @brief   Enables the PAL subsystem.
 */
#ifndef HAL_USE_PAL
#define HAL_USE_PAL                 FALSE
#endif

/**
 * @brief   Enables the ADC subsystem.
 */
#ifndef HAL_USE_ADC
#define HAL_USE_ADC                 FALSE
#endif

/**
 * @brief   Enables the CAN subsystem.
 */
#ifndef HAL_USE_CAN
#define HAL_USE_CAN                 FALSE
#endif

/**
 * @brief   Enables the DAC subsystem.
 */
#ifndef HAL_USE_DAC
#define HAL_USE_DAC                 FALSE
#endif

/**
 * @brief   Enables the EXT subsystem.
 */
#ifndef HAL_USE_EXT
#define HAL_USE_EXT                 FALSE
#endif

/**
 * @brief   Enables the GPT subsystem.
 */
#ifndef HAL_USE_GPT
#define HAL_USE_GPT                 FALSE
#endif

/**
 * @brief   Enables the I2C subsystem.
 */
#ifndef HAL_USE_I2C
#define HAL_USE_I2C                 FALSE
#endif

/**
 * @brief   Enables the I2S subsystem.
 */
#ifndef HAL_USE_I2S
#define HAL_USE_I2S                 FALSE
#endif

/**
 * @brief   Enables the ICU subsystem.
 */
#ifndef HAL_USE_ICU
#define HAL_USE_ICU                 FALSE
#endif

/**
 * @brief   Enables the MAC subsystem.
 */
#ifndef HAL_USE_MAC
#define HAL_USE_MAC                 FALSE
#endif

/**
 * @brief   Enables the MMC_SPI subsystem.
 */
#ifndef HAL_USE_MMC_SPI
#define HAL_USE_MMC_SPI             FALSE
#endif

/**
 * @brief   Enables the PWM subsystem.
 */
#ifndef HAL_USE_PWM
#define HAL_USE_PWM                 FALSE
#endif

/**
 * @brief   Enables the RTC subsystem.
 */
#ifndef HAL_USE_RTC
#define HAL_USE_RTC                 FALSE
#endif

/**
 * @brief   Enables the SDC subsystem.
 */
#ifndef HAL_USE_SDC
#define HAL_USE_SDC                 FALSE
#endif

/**
 * @brief   Enables the SERIAL subsystem.
 */
#ifndef HAL_USE_SERIAL
#define HAL_USE_SERIAL              FALSE
#endif

/**
 * @brief   Enables the SERIAL over USB subsystem.
 */
#ifndef HAL_USE_SERIAL_USB
#define HAL_USE_SERIAL_USB          FALSE
#endif

/**
 * @brief   Enables the SPI subsystem.
 */
#ifndef HAL_USE_SPI
#define HAL_USE_SPI                 TRUE
#endif

/**
 * @brief   Enables the UART subsystem.
 */
#ifndef HAL_USE_UART
#define HAL_USE_UART                FALSE
#endif

/**
 * @brief   Enables the USB subsystem.
 */
#ifndef HAL_USE_USB
#define HAL_USE_USB                 FALSE
#endif

/*===========================================================================*/
/* SERIAL driver related settings.                                           */
/*===========================================================================*/

/**
 * @brief   Default bit rate.
 * @details Configuration parameter, this is the baud rate selected for the
 *          default configuration.
 */
#ifndef SERIAL_DEFAULT_BITRATE
#define SERIAL_DEFAULT_BITRATE      38400
#endif

/**
 * @brief   Serial buffers size.
 * @details Configuration parameter, you can change the depth of the queue
 *          buffers depending on the requirements of your application.
 * @note    The default is 64 bytes for both the transmission and receive
 *          buffers.
 */
#ifndef SERIAL_BUFFERS_SIZE
#define SERIAL_BUFFERS_SIZE         16
#endif

/*===========================================================================*/
/* SERIAL_USB driver related setting.                                        */
/*===========================================================================*/

/**
 * @brief   Serial over USB buffers size.
 * @details Configuration parameter, the buffer size must be a multiple of
 *          the USB data endpoint maximum packet size.
 * @note    The default is 64 bytes for both the transmission and receive
 *          buffers.
 */
#ifndef SERIAL_USB_BUFFERS_SIZE
#define SERIAL_USB_BUFFERS_SIZE     256
#endif

/*===========================================================================*/
/* SPI driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Enables synchronous APIs.
 * @note    Disabling this option saves both code and data space.
 */
#ifndef SPI_USE_WAIT
#define SPI_USE_WAIT                TRUE
#endif

/**
 * @brief   Enables the @p spiAcquireBus() and @p spiReleaseBus() APIs.
 * @note    Disabling this option saves both code and data space.
 */
#ifndef SPI_USE_MUTUAL_EXCLUSION
#define SPI_USE_MUTUAL_EXCLUSION    TRUE
#endif

#endif /* _HALCONF_H_ */

/** @} */
