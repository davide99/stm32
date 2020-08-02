#include "../lib/Serial.h"

int main() {
    Serial serial(SerialN::S3, 115200);

    while (true) {
        serial.write(serial.read());
    }
}