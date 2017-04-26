//
// Created by renaud on 4/26/17.
//

#ifndef _GPIO_H
#define _GPIO_H

#include "hal.h"
#include "gpio_lld.h"

#define gpio_configure(gpio)    _pal_lld_setgroupmode(gpio_port(gpio), gpio_pin(gpio), gpio_mode(gpio))

#define gpio_port(gpio)         _gpio_lld_port(gpio)
#define gpio_pin(gpio)          _gpio_lld_pin(gpio)
#define gpio_mode(gpio)         _gpio_lld_mode(gpio)

#define gpioSetPad(gpio)        palSetPad(gpio_port(gpio), gpio_pin(gpio))
#define gpioClearPad(gpio)      palClearPad(gpio_port(gpio), gpio_pin(gpio))

#endif //_GPIO_H
