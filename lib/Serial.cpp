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

#define USART_CR1_RE     ((uint32_t)(1u << 2u))
#define USART_CR1_TE     ((uint32_t)(1u << 3u))
#define USART_CR1_RXNEIE ((uint32_t)(1u << 5u))
#define USART_CR1_UE     ((uint32_t)(1u << 13u))

#define USART_SR_RXNE ((uint32_t)(1u << 5u))
#define USART_SR_TC   ((uint32_t)(1u << 6u))
#define USART_SR_TXE  ((uint32_t)(1u << 7u))

Serial::Serial(SerialN n, uint32_t baudRate, bool enableRxInterrupt, Interrupts::Priority priority) {
    Interrupts::Interrupt interrupt;

    switch (n) {
        case SerialN::S1: {
            Utils::enablePeripheral(Utils::Peripheral::Usart1);
            //TX: A9, RX: A10
            Utils::enablePeripheral(Utils::Peripheral::PortA);
            GPIO::setOutPin(GPIO::Pin::A9, GPIO::OutMode::AltPushPull);
            GPIO::setInPin(GPIO::Pin::A10, GPIO::InMode::Floating);

            this->baseAddress = USART1_BASE_ADDR;
            interrupt = Interrupts::Usart1;
            break;
        }
        case SerialN::S2: {
            Utils::enablePeripheral(Utils::Peripheral::Usart2);
            //TX: A2, RX: A3
            Utils::enablePeripheral(Utils::Peripheral::PortA);
            GPIO::setOutPin(GPIO::Pin::A2, GPIO::OutMode::AltPushPull);
            GPIO::setInPin(GPIO::Pin::A3, GPIO::InMode::Floating);

            this->baseAddress = USART2_BASE_ADDR;
            interrupt = Interrupts::Usart2;
            break;
        }
        case SerialN::S3: {
            Utils::enablePeripheral(Utils::Peripheral::Usart3);
            //TX: B10, RX: B11
            Utils::enablePeripheral(Utils::Peripheral::PortB);
            GPIO::setOutPin(GPIO::Pin::B10, GPIO::OutMode::AltPushPull);
            GPIO::setInPin(GPIO::Pin::B11, GPIO::InMode::Floating);

            this->baseAddress = USART3_BASE_ADDR;
            interrupt = Interrupts::Usart3;
            break;
        }
    }

    if (enableRxInterrupt) {
        Interrupts::setPriority(interrupt, priority);
        Interrupts::enable(interrupt);
    }

    USART_BRR(this->baseAddress) = F_CPU / baudRate;
    USART_CR1(this->baseAddress) =
            USART_CR1_RE | USART_CR1_TE | USART_CR1_UE | (enableRxInterrupt ? USART_CR1_RXNEIE : 0u);
}

uint8_t Serial::read() const {
    while (!(USART_SR(this->baseAddress) & USART_SR_RXNE));
    return USART_DR(this->baseAddress);
}

void Serial::print(uint8_t byte) const {
    while (!(USART_SR(this->baseAddress) & USART_SR_TXE));
    USART_DR(this->baseAddress) = byte;
    while (!(USART_SR(this->baseAddress) & USART_SR_TC));
}

void Serial::close() const {
    USART_CR1(this->baseAddress) &= ~USART_CR1_RE;
}

void Serial::print(char *str) const {
    while (*str) {
        while (!(USART_SR(this->baseAddress) & USART_SR_TXE));
        USART_DR(this->baseAddress) = *str;
        while (!(USART_SR(this->baseAddress) & USART_SR_TC));
        str++;
    }
}

void Serial::print(const std::string &string) const {
    print(const_cast<char *>(string.c_str()));
}


void Serial::print(int val, int base) const {
    char num[] = "0123456789ABCDEF";
    char buff[20];
    int pos = 0;

    if (val < 0) {
        print('-');
        val = -val;
    }

    switch (base) {
        case 2:
            print("0b");
            break;
        case 8:
            print('0');
            break;
        case 16:
            print("0x");
            break;
        default:
            break;
    }

    while (val) {
        buff[pos++] = num[val % base];
        val /= base;
    }

    buff[pos] = 0;
    Utils::reverseString(buff, pos);
    print(buff);
}
