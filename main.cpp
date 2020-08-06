#include "lib/Serial.h"

int main() {
    Serial serial(SerialN::S3, 115200);
    serial.print("Uart initialized\n");
    serial.print(200);

    while (true) {

    }
}
