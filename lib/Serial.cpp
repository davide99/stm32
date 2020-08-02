#include "Serial.h"
#include "Utils.h"
#include "GPIO.h"

#ifndef F_CPU
#define F_CPU (8000000ul)
#warning Assuming F_CPU = 8 MHz
#endif

#include "common.h"

#define USART1_BASE_ADDR 0x40013800u
#define USART2_BASE_ADDR 0x40004400u
#define USART3_BASE_ADDR 0x40004800u

#define USART_SR(base)  __RMM(base + 0x00u)
#define USART_DR(base)  __RMM(base + 0x04u)
#define USART_BRR(base) __RMM(base + 0x08u)
#define USART_CR1(base) __RMM(base + 0x0Cu)

Serial::Serial(SerialN n, uint32_t baudRate, bool enableRxInterrupt) {
    switch (n) {
        case SerialN::S1: {
            Utils::enablePeripheral(Utils::Peripheral::Usart1);
            //TX: A9, RX: A10
            Utils::enablePeripheral(Utils::Peripheral::PortA);
            GPIO::setOutPin(GPIO::Pin::A9, GPIO::OutMode::AltPushPull);
            GPIO::setInPin(GPIO::Pin::A10, GPIO::InMode::Floating);

            this->baseAddress = USART1_BASE_ADDR;
            break;
        }
        case SerialN::S2: {
            Utils::enablePeripheral(Utils::Peripheral::Usart2);
            //TX: A2, RX: A3
            Utils::enablePeripheral(Utils::Peripheral::PortA);
            break;
        }
        case SerialN::S3: {
            Utils::enablePeripheral(Utils::Peripheral::Usart3);
            //TX: B10, RX: B11
            Utils::enablePeripheral(Utils::Peripheral::PortB);
            break;
        }
    }

    USART_BRR(this->baseAddress) = F_CPU / baudRate;
    USART_CR1(this->baseAddress) = 0x200Cu;
}

uint8_t Serial::readByte() const {
    while (!(USART_SR(this->baseAddress) & (1u << 5u)));
    return USART_DR(this->baseAddress);
}

void Serial::printByte(uint8_t byte) const {
    while (!(USART_SR(this->baseAddress) & (1u << 7u)));
    USART_DR(this->baseAddress) = byte;
    while (!(USART_SR(this->baseAddress) & (1u << 6u)));
}
