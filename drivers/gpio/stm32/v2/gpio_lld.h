#ifndef GPIO_LLD_H
#define GPIO_LLD_H

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/

#undef GPIO_MODE_RESET
#undef GPIO_MODE_UNCONNECTED
#undef GPIO_MODE_INPUT
#undef GPIO_MODE_INPUT_PULLUP
#undef GPIO_MODE_INPUT_PULLDOWN
#undef GPIO_MODE_INPUT_ANALOG
#undef GPIO_MODE_OUTPUT_PUSHPULL
#undef GPIO_MODE_OUTPUT_OPENDRAIN

/*===========================================================================*/
/* STM32-specific I/O mode flags                                             */
/*===========================================================================*/
#define GPIO_STM32_MODE_MASK             (3U << 0U)
#define GPIO_STM32_MODE_INPUT            (0U << 0U)
#define GPIO_STM32_MODE_OUTPUT           (1U << 0U)
#define GPIO_STM32_MODE_ALTERNATE        (2U << 0U)
#define GPIO_STM32_MODE_ANALOG           (3U << 0U)

#define GPIO_STM32_OTYPE_MASK            (1U << 2U)
#define GPIO_STM32_OTYPE_PUSHPULL        (0U << 2U)
#define GPIO_STM32_OTYPE_OPENDRAIN       (1U << 2U)

#define GPIO_STM32_OSPEED_MASK           (3U << 3U)
#define GPIO_STM32_OSPEED_LOWEST         (0U << 3U)
#if defined(STM32F0XX) || defined(STM32F30X) || defined(STM32F37X)
#define GPIO_STM32_OSPEED_MID            (1U << 3U)
#else
#define GPIO_STM32_OSPEED_MID1           (1U << 3U)
#define GPIO_STM32_OSPEED_MID2           (2U << 3U)
#endif
#define GPIO_STM32_OSPEED_HIGHEST        (3U << 3U)

#define GPIO_STM32_PUPDR_MASK            (3U << 5U)
#define GPIO_STM32_PUPDR_FLOATING        (0U << 5U)
#define GPIO_STM32_PUPDR_PULLUP          (1U << 5U)
#define GPIO_STM32_PUPDR_PULLDOWN        (2U << 5U)

#define GPIO_STM32_ALTERNATE_MASK        (15U << 7U)
#define GPIO_STM32_ALTERNATE(n)          ((n) << 7U)


/*===========================================================================*/
/* Standard I/O mode flags                                                   */
/*===========================================================================*/
#define GPIO_MODE_ALTERNATE(n)           (GPIO_STM32_MODE_ALTERNATE | GPIO_STM32_ALTERNATE(n))
#define GPIO_MODE_RESET                  GPIO_STM32_MODE_INPUT
#define GPIO_MODE_UNCONNECTED            GPIO_MODE_INPUT_PULLUP
#define GPIO_MODE_INPUT                  GPIO_STM32_MODE_INPUT
#define GPIO_MODE_INPUT_PULLUP           (GPIO_STM32_MODE_INPUT | GPIO_STM32_PUPDR_PULLUP)
#define GPIO_MODE_INPUT_PULLDOWN         (GPIO_STM32_MODE_INPUT | GPIO_STM32_PUPDR_PULLDOWN)
#define GPIO_MODE_INPUT_ANALOG           GPIO_STM32_MODE_ANALOG
#define GPIO_MODE_OUTPUT_PUSHPULL        (GPIO_STM32_MODE_OUTPUT | GPIO_STM32_OTYPE_PUSHPULL)
#define GPIO_MODE_OUTPUT_OPENDRAIN       (GPIO_STM32_MODE_OUTPUT | GPIO_STM32_OTYPE_OPENDRAIN)

/*===========================================================================*/
/* GPIO ports definitions                                                    */
/*===========================================================================*/
#undef GPIOA
#undef GPIOB
#undef GPIOC
#undef GPIOD
#undef GPIOE
#undef GPIOF
#undef GPIOG
#undef GPIOH
#undef GPIOI
#undef GPIOJ
#undef GPIOK

#if STM32_HAS_GPIOA
#define GPIOA               0
#endif
#if STM32_HAS_GPIOB
#define GPIOB               1
#endif
#if STM32_HAS_GPIOC
#define GPIOC               2
#endif
#if STM32_HAS_GPIOD
#define GPIOD               3
#endif
#if STM32_HAS_GPIOE
#define GPIOE               4
#endif
#if STM32_HAS_GPIOF
#define GPIOF               5
#endif
#if STM32_HAS_GPIOG
#define GPIOG               6
#endif
#if STM32_HAS_GPIOH
#define GPIOH               7
#endif
#if STM32_HAS_GPIOI
#define GPIOI               8
#endif
#if STM32_HAS_GPIOJ
#define GPIOJ               9
#endif
#if STM32_HAS_GPIOK
#define GPIOK               10
#endif

/*===========================================================================*/
/* STM32 GPIO registers block.                                               */
/*===========================================================================*/
typedef struct {

    volatile uint32_t     MODER;
    volatile uint32_t     OTYPER;
    volatile uint32_t     OSPEEDR;
    volatile uint32_t     PUPDR;
    volatile uint32_t     IDR;
    volatile uint32_t     ODR;
    volatile union {
        uint32_t          W;
        struct {
            uint16_t      set;
            uint16_t      clear;
        } H;
    } BSRR;
    volatile uint32_t     LCKR;
    volatile uint32_t     AFRL;
    volatile uint32_t     AFRH;
    volatile uint32_t     BRR;
} stm32_gpio_t;


/*===========================================================================*/
/* GPIO declaration macros                                                   */
/*===========================================================================*/

#define GPIO_BITS_PAD_SIZE                  8
#define GPIO_BITS_PORT_SIZE                 8
#define GPIO_BITS_PAD_OFFSET                0
#define GPIO_BITS_PORT_OFFSET               (GPIO_BITS_PAD_OFFSET + GPIO_BITS_PAD_SIZE)

#define GPIO_BITS_PAD_MASK                  ((1 << (GPIO_BITS_PAD_SIZE)) - 1)
#define GPIO_BITS_PORT_MASK                 ((1 << (GPIO_BITS_PORT_SIZE)) - 1)

#define _GPIO_SET(NAME, v)                  (((v) & (GPIO_BITS_##NAME##_MASK)) << (GPIO_BITS_##NAME##_OFFSET))
#define _GPIO_GET(NAME, v)                  (((v) >> GPIO_BITS_##NAME##_OFFSET) & GPIO_BITS_##NAME##_MASK)

#define GPIO_PORT(gpio)                     _GPIO_GET(PORT, gpio)
#define GPIO_PAD(gpio)                      _GPIO_GET(PAD, gpio)

#define DECLARE_GPIO(port, pad)             (_GPIO_SET(PORT, port) | _GPIO_SET(PAD, pad))

#define GPIO_A(pad)                         DECLARE_GPIO(0, pad)
#define GPIO_B(pad)                         DECLARE_GPIO(1, pad)
#define GPIO_C(pad)                         DECLARE_GPIO(2, pad)
#define GPIO_D(pad)                         DECLARE_GPIO(3, pad)
#define GPIO_E(pad)                         DECLARE_GPIO(4, pad)
#define GPIO_F(pad)                         DECLARE_GPIO(5, pad)
#define GPIO_G(pad)                         DECLARE_GPIO(6, pad)
#define GPIO_H(pad)                         DECLARE_GPIO(7, pad)
#define GPIO_I(pad)                         DECLARE_GPIO(8, pad)
#define GPIO_J(pad)                         DECLARE_GPIO(9, pad)
#define GPIO_K(pad)                         DECLARE_GPIO(10, pad)

/*===========================================================================*/
/* Typedefs                                                                  */
/*===========================================================================*/
extern stm32_gpio_t * __stm32_ports[];

typedef uint32_t gpio_t;
typedef uint8_t  ioportid_t;
typedef uint16_t ioportmask_t;
typedef uint32_t iomode_t;

/*===========================================================================*/
/* Implementations                                                           */
/*===========================================================================*/

#define _gpio_lld_readport(port)            (__stm32_ports[(port)]->IDR)
#define _gpio_lld_writeport(port, bits)     (__stm32_ports[(port)]->ODR = (bits))
#define _gpio_lld_setport(port, bits)       (__stm32_ports[(port)]->BSRR.H.set = (uint16_t)(bits))
#define _gpio_lld_clearport(port, bits)     (__stm32_ports[(port)]->BSRR.H.clear = (uint16_t)(bits))

void _gpio_lld_init(void);
void _gpio_lld_setportmode(ioportid_t port, ioportmask_t mask, iomode_t mode);

#endif //GPIO_LLD_H
