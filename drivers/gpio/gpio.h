#ifndef _GPIO_H
#define _GPIO_H

#ifndef _HAL_H_
#error you should not include <gpio.h> directly, include <hal.h> instead.
#endif

#if HAL_USE_PAL == TRUE
#error this driver is not compatible with the pal driver.
#endif

/**
 * Local definitions that can be overridden by low-level drivers
 */
#define GPIO_MODE_RESET                             0
#define GPIO_MODE_UNCONNECTED                       1
#define GPIO_MODE_INPUT                             2
#define GPIO_MODE_INPUT_PULLUP                      3
#define GPIO_MODE_INPUT_PULLDOWN                    4
#define GPIO_MODE_INPUT_ANALOG                      5
#define GPIO_MODE_OUTPUT_PUSHPULL                   6
#define GPIO_MODE_OUTPUT_OPENDRAIN                  7

#include "gpio_lld.h"

#define gpioInit()                                  _gpio_lld_init()
#define gpioReadPort(port)                          _gpio_lld_readport(port)
#define gpioWritePort(port, bits)                   _gpio_lld_writeport(port, bits)
#define gpioSetPort(port, mask)                     _gpio_lld_setport(port, mask)
#define gpioClearPort(port, mask)                   _gpio_lld_clearport(port, mask)
#define gpioSetPortMode(port, mask, mode)           _gpio_lld_setportmode(port, mask, mode)

#define gpioReadPad(gpio)                           ((gpioReadPort(GPIO_PORT(gpio)) >> GPIO_PAD(gpio)) & 1)
#define gpioSetPad(gpio)                            gpioSetPort(GPIO_PORT(gpio), 1 << GPIO_PAD(gpio))
#define gpioClearPad(gpio)                          gpioClearPort(GPIO_PORT(gpio), 1 << GPIO_PAD(gpio))
#define gpioSetPadMode(gpio, mode)                  gpioSetPortMode(GPIO_PORT(gpio), 1 << GPIO_PAD(gpio), mode)

// Compatibility layer for pal driver
#define palReadPort(port)                           gpioReadPort(port)
#define palWritePort(port, bits)                    gpioWritePort(port, bits)
#define palSetPort(port, mask)                      gpioSetPort(port, mask)
#define palClearPort(port, mask)                    gpioClearPort(port, mask)
#define palSetGroupMode(port, mask, offset, mode)   gpioSetPortMode(port, (mask) << (offset), mode)

#define palReadPad(port, pad)                       ((gpioReadPort(gpio) >> (pad)) & 1)
#define palSetPad(port, pad)                        gpioSetPort(port, 1 << (pad))
#define palClearPad(port, pad)                      gpioClearPort(port, 1 << (pad))
#define palSetPadMode(port, pad, mode)              gpioSetPortMode(port, 1 << (pad), mode)

#endif //_GPIO_H
