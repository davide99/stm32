#include "lib/Serial.h"
#include "lib/Clock.h"

int main() {
    Serial serial(SerialN::S3, 115200);
    serial.print("Uart initialized\n");

    serial.print("Clock speed: ");
    serial.print(Clock::getSystemClockMHz(), 10);
    serial.print("\nAHB speed: ");
    serial.print(Clock::getAHBClockMHz(), 10);
    serial.print("\n");

    while (true) {
        __asm__("nop");
    }
}