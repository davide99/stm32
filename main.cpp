#include "lib/GPIO.h"
#include "lib/Utils.h"

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
    Utils::enablePeripheral(Utils::Peripheral::PortA);
    Utils::enablePeripheral(Utils::Peripheral::PortC);

    GPIO::setOutPin(GPIO::Pin::C13);
    GPIO::setInPin(GPIO::Pin::A0, GPIO::InMode::PullDown);

    //Alternative function needs to be enabled to make interrupts work
    Utils::enablePeripheral(Utils::Peripheral::AFIO);
    GPIO::setupInterrupt(GPIO::Pin::A0, GPIO::IntTrigger::Rising);

    flag = false;

    while (true) {
        if (flag) {
            GPIO::toggle(GPIO::Pin::C13);
            flag = false;
        }
    }
}