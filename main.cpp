#include "lib/GPIO.h"

/*
 * +---------+            ^
 * |      C13|---LED      |
 * |      C14|--------BTN-+
 * |         |
 * |  STM32  |
 * +---------+
 */

volatile bool entrato;

extern "C" {
    void EXTI0_IRQ_handler(void) {
        entrato = true;
        GPIO::clearPendingInterrupt(GPIO::Pin::A0);
    }
}

int main() {
    GPIO::enablePort(GPIO::Port::A);
    GPIO::enablePort(GPIO::Port::C);

    GPIO::setOutPin(GPIO::Pin::C13);
    GPIO::setInPin(GPIO::Pin::A0, GPIO::InMode::PullDown);

    GPIO::enableAlternativeFunction();
    GPIO::setupInterrupt(GPIO::Pin::A0, GPIO::IntTrigger::Rising, GPIO::IntPriority::P10);

    entrato = false;

    while (true) {
        if(entrato){
            GPIO::toggle(GPIO::Pin::C13);
            entrato = false;
        }
    }
}