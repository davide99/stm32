#include "Serial.h"
#include "Utils.h"
#include "GPIO.h"

#ifndef F_CPU
#define F_CPU (8000000ul)
#warning Assuming F_CPU = 8 MHz
#endif

#include "common.h"

#define USART1_SR __RMM(0x40013800u + 0x00u)
#define USART1_DR __RMM(0x40013800u + 0x04u)
#define USART1_BRR __RMM(0x40013800u + 0x08u)
#define USART1_CR1 __RMM(0x40013800u + 0x0Cu)

Serial::Serial(SerialN n, uint32_t baudRate) {
    this->n = n;

    switch (n) {
        case SerialN::S1:
            //Utils::enablePeripheral(Utils::Peripheral::AFIO);
            Utils::enablePeripheral(Utils::Peripheral::Usart1);
            //TX: A9, RX: A10
            Utils::enablePeripheral(Utils::Peripheral::PortA);
            GPIO::setOutPin(GPIO::Pin::A9, GPIO::OutMode::AltPushPull);
            GPIO::setInPin(GPIO::Pin::A10, GPIO::InMode::Floating);

            uint32_t uartDiv = F_CPU / baudRate;
            USART1_BRR = uartDiv;
            USART1_CR1 = 0x200Cu;
            break;
            /*case SerialN::S2:
                Utils::enablePeripheral(Utils::Peripheral::Usart2);
                //TX: A2, RX: A3
                Utils::enablePeripheral(Utils::Peripheral::PortA);
                break;
            case SerialN::S3:
                Utils::enablePeripheral(Utils::Peripheral::Usart3);
                //TX: B10, RX: A11
                Utils::enablePeripheral(Utils::Peripheral::PortB);
                break;*/
    }


}

uint8_t Serial::readByte() {
    while (!(USART1_SR & (1u << 5u)));
    return USART1_DR;
}

void Serial::printByte(uint8_t byte) {
    while (!(USART1_SR & (1u << 7u)));
    USART1_DR = byte;
    while (!(USART1_SR & (1u << 6u)));
}
