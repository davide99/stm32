#include "Timer2.h"
#include "Utils.h"
#include "common.h"

#define TIM2_BASE 0x40000000u

#define TIM2_CR1 __RMM(TIM2_BASE + 0x00u)
#define TIM_CR1_CEN (1u << 0u)

#define TIM2_PSC __RMM(TIM2_BASE + 0x28u)

Timer2::Timer2(Interrupts::Priority priority) {
    Utils::enablePeripheral(Utils::Peripheral::Timer2);
    Interrupts::setPriority(Interrupts::Tim2, priority);
    Interrupts::enable(Interrupts::Tim2);

    //Make sure the timer is off
    this->disable();

    //Reset
    Utils::resetPeripheral(Utils::Peripheral::Timer2);
}

void Timer2::disable() {
    TIM2_CR1 &= ~TIM_CR1_CEN;
}
