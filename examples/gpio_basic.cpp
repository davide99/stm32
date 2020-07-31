#include "../lib/GPIO.h"

/*
 * +---------+            ^
 * |      C13|---LED      |
 * |      C14|--------BTN-+
 * |         |
 * |  STM32  |
 * +---------+
 */

int main() {
    GPIO::enablePort(GPIO::Port::C);

    GPIO::setOutPin(GPIO::Pin::C13);
    GPIO::setInPin(GPIO::Pin::C14, GPIO::InMode::PullDown);

    while (true) {
        GPIO::digitalWrite(GPIO::Pin::C13, GPIO::digitalRead(GPIO::Pin::C14));
    }
}