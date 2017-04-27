#include <hal.h>

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#if defined(STM32L0XX) || defined(STM32L1XX)
#define AHB_EN_MASK     STM32_GPIO_EN_MASK
#define AHB_LPEN_MASK   AHB_EN_MASK

#elif defined(STM32F0XX) || defined(STM32F3XX) || defined(STM32F37X)
#define AHB_EN_MASK     STM32_GPIO_EN_MASK
#define AHB_LPEN_MASK   0

#elif defined(STM32F2XX) || defined(STM32F4XX) || defined(STM32F7XX)
#define AHB1_EN_MASK    STM32_GPIO_EN_MASK
#define AHB1_LPEN_MASK  AHB1_EN_MASK

#else
#error "missing or unsupported platform for GPIOv2 PAL driver"
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
stm32_gpio_t * __stm32_ports[] = {
#if STM32_HAS_GPIOA
        (stm32_gpio_t *)GPIOA_BASE,
#endif
#if STM32_HAS_GPIOB
        (stm32_gpio_t *)GPIOB_BASE,
#endif
#if STM32_HAS_GPIOC
        (stm32_gpio_t *)GPIOC_BASE,
#endif
#if STM32_HAS_GPIOD
        (stm32_gpio_t *)GPIOD_BASE,
#endif
#if STM32_HAS_GPIOE
        (stm32_gpio_t *)GPIOE_BASE,
#endif
#if STM32_HAS_GPIOF
        (stm32_gpio_t *)GPIOF_BASE,
#endif
#if STM32_HAS_GPIOG
        (stm32_gpio_t *)GPIOG_BASE,
#endif
#if STM32_HAS_GPIOH
        (stm32_gpio_t *)GPIOH_BASE,
#endif
#if STM32_HAS_GPIOI
        (stm32_gpio_t *)GPIOI_BASE,
#endif
#if STM32_HAS_GPIOJ
        (stm32_gpio_t *)GPIOJ_BASE,
#endif
#if STM32_HAS_GPIOK
        (stm32_gpio_t *)GPIOK_BASE,
#endif
};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void _gpio_lld_init() {

    /*
     * Enables the GPIO related clocks.
     */
#if defined(STM32L0XX)
    RCC->IOPENR |= AHB_EN_MASK;
    RCC->IOPSMENR |= AHB_LPEN_MASK;
#elif defined(STM32L1XX)
    rccEnableAHB(AHB_EN_MASK, TRUE);
    RCC->AHBLPENR |= AHB_LPEN_MASK;
#elif defined(STM32F0XX)
    rccEnableAHB(AHB_EN_MASK, TRUE);
#elif defined(STM32F3XX) || defined(STM32F37X)
    rccEnableAHB(AHB_EN_MASK, TRUE);
#elif defined(STM32F2XX) || defined(STM32F4XX) || defined(STM32F7XX)
    RCC->AHB1ENR   |= AHB1_EN_MASK;
    RCC->AHB1LPENR |= AHB1_LPEN_MASK;
#endif
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    @p GPIO_MODE_UNCONNECTED is implemented as push pull at minimum
 *          speed.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 *
 * @notapi
 */
void _gpio_lld_setportmode(ioportid_t portid, ioportmask_t mask, iomode_t mode) {

    stm32_gpio_t * port = __stm32_ports[portid];

    uint32_t moder   = (mode & GPIO_STM32_MODE_MASK) >> 0;
    uint32_t otyper  = (mode & GPIO_STM32_OTYPE_MASK) >> 2;
    uint32_t ospeedr = (mode & GPIO_STM32_OSPEED_MASK) >> 3;
    uint32_t pupdr   = (mode & GPIO_STM32_PUPDR_MASK) >> 5;
    uint32_t altr    = (mode & GPIO_STM32_ALTERNATE_MASK) >> 7;
    uint32_t bit     = 0;

    while (true) {
        if ((mask & 1) != 0) {
            uint32_t altrmask, m1, m2, m4;

            altrmask = altr << ((bit & 7) * 4);
            m1 = 1 << bit;
            m2 = 3 << (bit * 2);
            m4 = 15 << ((bit & 7) * 4);
            port->OTYPER  = (port->OTYPER & ~m1) | otyper;
            port->OSPEEDR = (port->OSPEEDR & ~m2) | ospeedr;
            port->PUPDR   = (port->PUPDR & ~m2) | pupdr;
            if ((mode & GPIO_STM32_MODE_MASK) == GPIO_STM32_MODE_ALTERNATE) {
                /* If going in alternate mode then the alternate number is set
                   before switching mode in order to avoid glitches.*/
                if (bit < 8)
                    port->AFRL = (port->AFRL & ~m4) | altrmask;
                else
                    port->AFRH = (port->AFRH & ~m4) | altrmask;
                port->MODER   = (port->MODER & ~m2) | moder;
            }
            else {
                /* If going into a non-alternate mode then the mode is switched
                   before setting the alternate mode in order to avoid glitches.*/
                port->MODER   = (port->MODER & ~m2) | moder;
                if (bit < 8)
                    port->AFRL = (port->AFRL & ~m4) | altrmask;
                else
                    port->AFRH = (port->AFRH & ~m4) | altrmask;
            }
        }
        mask >>= 1;
        if (!mask) {
            return;
        }
        otyper <<= 1;
        ospeedr <<= 2;
        pupdr <<= 2;
        moder <<= 2;
        bit++;
    }
}
