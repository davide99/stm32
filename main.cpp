#include "lib/GPIO.h"

/*
 * +---------+            ^
 * |      C13|---LED      |
 * |      C14|--------BTN-+
 * |         |
 * |  STM32  |
 * +---------+
 */

void EXTI15_10_IRQ_handler() {
    GPIO::toggle(GPIO::Pin::C13);
    GPIO::clearPendingInterrupt(GPIO::Pin::C14);
}

int main() {
    GPIO::enablePort(GPIO::Port::C);

    GPIO::setOutPin(GPIO::Pin::C13);
    GPIO::setInPin(GPIO::Pin::C14, GPIO::InMode::PullDown);
    GPIO::setupInterrupt(GPIO::Pin::C14, GPIO::IntTrigger::Rising);

    while (true);
}