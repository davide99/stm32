#include "lib/Serial.h"
#include "lib/SPI.h"

int main() {
    Serial serial(SerialN::S3, 115200);
    serial.print("Uart initialized\n");
    SPI spi(SPIn::SPI1, GPIO::Pin::B12);

    while (true) {
        __asm__("nop");
    }
}