#include "lib/GPIO.h"

int main() {
    GPIO::enablePort(GPIO::Port::C);

    GPIO::pinMode(GPIO::Pin::C13, GPIO::Mode::OUT_PUSH_PULL);

    while (true) {
        GPIO::toggle(GPIO::Pin::C13);
        for (int i = 0; i < 500000; i++);
    }
}