#ifndef STM32_INTERRUPTS_H
#define STM32_INTERRUPTS_H

#include <cstdint>

namespace Interrupts {
    /*
     * RM008, pag 205
     * TODO: add other interrupts...
     */
    enum Interrupt : uint8_t {
        Exti0 = 6u,
        Exti1 = 7u,
        Exti2 = 8u,
        Exti3 = 9u,
        Exti4 = 10u,
        Exti5_9 = 23u,
        Usart1 = 37u,
        Usart2 = 38u,
        Usart3 = 39u,
        Exti10_15 = 40u,
    };

    enum class Priority : uint8_t {
        P0 = 0u,
        Max = P0,
        P1 = 1u,
        P2, P3, P4, P5, P6, P7, P8, P9,
        P10, P11, P12, P13, P14, P15,
        Min = P15,
        Default
    };

    void setPriority(enum Interrupt interrupt, enum Priority priority);

    void enable(enum Interrupt interrupt);
}

#endif
