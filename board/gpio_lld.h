#ifndef GPIO_LLD_H
#define GPIO_LLD_H

extern const ioportid_t __gpio_ports[];

typedef uint32_t gpio_t;

#define GPIO_PIN_BIT_SIZE           8
#define GPIO_PIN_BIT_OFFSET         0

#define GPIO_PORT_BIT_SIZE          8
#define GPIO_PORT_BIT_OFFSET        (GPIO_PIN_BIT_OFFSET + GPIO_PIN_BIT_SIZE)

#define GPIO_MODE_BIT_SIZE          16
#define GPIO_MODE_BIT_OFFSET        (GPIO_PORT_BIT_OFFSET + GPIO_PORT_BIT_SIZE)

#define GPIO_PIN_BIT_MASK           ((1 << (GPIO_PIN_BIT_SIZE)) - 1)
#define GPIO_PORT_BIT_MASK          ((1 << (GPIO_PORT_BIT_SIZE)) - 1)
#define GPIO_MODE_BIT_MASK          ((1 << (GPIO_MODE_BIT_SIZE)) - 1)

#define _GPIO_SET(NAME, v)           (((v) & (GPIO_##NAME##_BIT_MASK)) << (GPIO_##NAME##_BIT_OFFSET))
#define _GPIO_GET(NAME, v)           (((v) >> GPIO_##NAME##_BIT_OFFSET) & GPIO_##NAME##_BIT_MASK)

#define GPIO(port, pin, mode)       (_GPIO_SET(PORT, port) | _GPIO_SET(PIN, pin) | _GPIO_SET(MODE, mode))

#define GPIO_A(pin, mode)            GPIO(0, pin, mode)
#define GPIO_B(pin, mode)            GPIO(1, pin, mode)
#define GPIO_C(pin, mode)            GPIO(2, pin, mode)
#define GPIO_D(pin, mode)            GPIO(3, pin, mode)
#define GPIO_E(pin, mode)            GPIO(4, pin, mode)
#define GPIO_F(pin, mode)            GPIO(5, pin, mode)
#define GPIO_G(pin, mode)            GPIO(6, pin, mode)
#define GPIO_H(pin, mode)            GPIO(7, pin, mode)
#define GPIO_I(pin, mode)            GPIO(8, pin, mode)
#define GPIO_J(pin, mode)            GPIO(9, pin, mode)
#define GPIO_K(pin, mode)            GPIO(10, pin, mode)

#define _gpio_lld_pin(gpio)         _GPIO_GET(PIN, gpio)
#define _gpio_lld_mode(gpio)        _GPIO_GET(MODE, gpio)
#define _gpio_lld_port(gpio)        __gpio_ports[_GPIO_GET(PORT, gpio)]

#endif //GPIO_LLD_H
