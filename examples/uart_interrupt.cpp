#include "../lib/Serial.h"

Serial serial(SerialN::S3, 115200, true);

extern "C" {
void USART3_IRQ_handler() {
    serial.write(serial.read());
}
}

int main(){
    while (true){
        __asm__("wfi");
    }
}
