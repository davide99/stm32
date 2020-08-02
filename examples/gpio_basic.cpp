#include "../lib/GPIO.h"
#include "../lib/Utils.h"

/*
 * +---------+            ^
 * |      C13|---LED      |
 * |      C14|--------BTN-+
 * |         |
 * |  STM32  |
 * +---------+
 */

int main() {
    Utils::enablePeripheral(Utils::Peripheral::PortC);

    GPIO::setOutPin(GPIO::Pin::C13);
    GPIO::setInPin(GPIO::Pin::C14, GPIO::InMode::PullDown);

    while (true) {
        GPIO::digitalWrite(GPIO::Pin::C13, GPIO::digitalRead(GPIO::Pin::C14));
    }
}