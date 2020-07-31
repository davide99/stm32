#include "GPIO.h"

#define __RMM(addr) (*(volatile uint32_t *)((uintptr_t)addr))

#define RCC_BASE 0x40021000u
#define RCC_APB2ENR __RMM(RCC_BASE + 0x18u)

#define GPIOA_BASE 0x40010800u
#define GPIO_OFFSET 0x400u      //GPIOA_BASE + GPIO_OFFSET = GPIOB_BASE

#define GPIO_CRL(GPIO_BASE)  __RMM(GPIO_BASE + 0x00u)
#define GPIO_CRH(GPIO_BASE)  __RMM(GPIO_BASE + 0x04u)
#define GPIO_IDR(GPIO_BASE)  __RMM(GPIO_BASE + 0x08u)
#define GPIO_ODR(GPIO_BASE)  __RMM(GPIO_BASE + 0x0Cu)

void GPIO::enablePort(GPIO::Port port) {
    switch (port) {
        case Port::A:
            RCC_APB2ENR |= 1u << 2u; //IOPA_EN
            break;
        case Port::B:
            RCC_APB2ENR |= 1u << 3u; //IOPB_EN
            break;
        case Port::C:
            RCC_APB2ENR |= 1u << 4u; //IOPC_EN
    }
}

void GPIO::pinMode(GPIO::Pin pin, GPIO::Mode mode, GPIO::OutSpeed outSpeed) {
    uint8_t pinValue = static_cast<uint8_t>(pin) & 0xFu; //0~15
    uint8_t portValue = static_cast<uint8_t>(pin) >> 4u; //0=A, 1=B, ...

    uintptr_t baseGpioAddr = GPIOA_BASE + portValue * GPIO_OFFSET;

    auto value = static_cast<uint8_t>(mode);
    if (!(value & 0b11u)) { //is input?
        value <<= 2u;
        value |= static_cast<uint8_t>(outSpeed);
    }

    if (pinValue <= 7u) {
        GPIO_CRL(baseGpioAddr) |= (value << ((pinValue << 2u) - 1u));
        GPIO_CRL(baseGpioAddr) &= (value << ((pinValue << 2u) - 1u));
    } else {
        GPIO_CRH(baseGpioAddr) |= (value << (((pinValue - 8u) << 2u) - 1u));
        GPIO_CRH(baseGpioAddr) &= (value << (((pinValue - 8u) << 2u) - 1u));
    }
}

bool GPIO::digitalRead(GPIO::Pin pin) {
    return false;
}

void GPIO::toggle(GPIO::Pin pin) {
    uint8_t pinValue = static_cast<uint8_t>(pin) & 0xFu; //0~15
    uint8_t portValue = static_cast<uint8_t>(pin) >> 4u; //0=A, 1=B, ...
    uintptr_t baseGpioAddr = GPIOA_BASE + portValue * GPIO_OFFSET;

    GPIO_ODR(baseGpioAddr) ^= (1u << pinValue);
}
