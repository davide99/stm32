#include "lib/Serial.h"
#include "lib/SPI.h"
#include "lib/SD.h"

int main() {
    Serial serial(SerialN::S3, 115200);
    serial.print("Uart initialized\n");
    SPI spi(SPIn::SPI1, GPIO::Pin::B12);
    SD sd(spi);

    uint8_t buff[512];
    for (uint8_t &i : buff)
        i = 0;

    sd.read(buff, 0);
    for (int i = 0; i < 512; i++) {
        if (i % 16 == 0)
            serial.print("\n");
        serial.print((uint8_t) buff[i]);
    }

    while (true) {
        __asm__("nop");
    }
}