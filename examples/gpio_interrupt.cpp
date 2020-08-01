#include "../lib/GPIO.h"

/*
 * +---------+            ^
 * |      C13|---LED      |
 * |       A0|--------BTN-+
 * |         |
 * |  STM32  |
 * +---------+
 */

volatile bool flag;

extern "C" {
void EXTI0_IRQ_handler(void) {
    flag = true;
    GPIO::clearPendingInterrupt(GPIO::Pin::A0);
}
}

int main() {
    GPIO::enablePort(GPIO::Port::A);
    GPIO::enablePort(GPIO::Port::C);

    GPIO::setOutPin(GPIO::Pin::C13);
    GPIO::setInPin(GPIO::Pin::A0, GPIO::InMode::PullDown);

    GPIO::enableAlternativeFunction();
    GPIO::setupInterrupt(GPIO::Pin::A0, GPIO::IntTrigger::Rising, GPIO::IntPriority::Default);

    flag = false;

    while (true) {
        if (flag) {
            GPIO::toggle(GPIO::Pin::C13);
            flag = false;
        }
    }
}