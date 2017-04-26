#include "gpio.h"

const ioportid_t __gpio_ports[] = {
#if STM32_HAS_GPIOA
        GPIOA,
#endif
#if STM32_HAS_GPIOB
        GPIOB,
#endif
#if STM32_HAS_GPIOC
        GPIOC,
#endif
#if STM32_HAS_GPIOD
        GPIOD,
#endif
#if STM32_HAS_GPIOE
        GPIOE,
#endif
#if STM32_HAS_GPIOF
        GPIOF,
#endif
#if STM32_HAS_GPIOG
        GPIOG,
#endif
#if STM32_HAS_GPIOH
        GPIOH,
#endif
#if STM32_HAS_GPIOI
        GPIOI,
#endif
#if STM32_HAS_GPIOJ
        GPIOJ,
#endif
#if STM32_HAS_GPIOK
        GPIOK,
#endif
};
