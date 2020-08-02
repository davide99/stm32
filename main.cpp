#include "lib/Serial.h"

int main() {
    Serial serial(SerialN::S1, 115200);

    while (true) {
        serial.printByte(serial.readByte());
    }
}