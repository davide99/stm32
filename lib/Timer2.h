#ifndef STM32_TIMER2_H
#define STM32_TIMER2_H

#include "Interrupts.h"

class Timer2 {
public:
    explicit Timer2(Interrupts::Priority priority = Interrupts::Priority::Default);

    void disable();
};


#endif
