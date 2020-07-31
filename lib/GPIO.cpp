#include "GPIO.h"

#include "common.h"

#define RCC_APB2ENR __RMM(0x40021000u + 0x18u)

#define GPIOA_BASE 0x40010800u
#define GPIO_OFFSET 0x400u      //GPIOA_BASE + GPIO_OFFSET = GPIOB_BASE

#define GPIO_CRL(GPIO_BASE)  __RMM(GPIO_BASE + 0x00u)
#define GPIO_CRH(GPIO_BASE)  __RMM(GPIO_BASE + 0x04u)
#define GPIO_IDR(GPIO_BASE)  __RMM(GPIO_BASE + 0x08u)
#define GPIO_ODR(GPIO_BASE)  __RMM(GPIO_BASE + 0x0Cu)

void GPIO::enablePort(GPIO::Port port) {
    RCC_APB2ENR |= 1u << static_cast<uint8_t>(port); //IOPx_EN
}

static void calcValues(GPIO::Pin pin, uint8_t &pinValue, uintptr_t &gpioBaseAddr) {
    /*
     * The pin value is the lowest nibble of pin,
     * because of how pin are stored in the enum
     */
    pinValue = static_cast<uint8_t>(pin) & 0xFu; //0~15

    /*
     * The port number is the highest nibble of pin,
     * for the same reason as above
     */
    uint8_t portValue = static_cast<uint8_t>(pin) >> 4u; //0=A, 1=B, ...

    gpioBaseAddr = GPIOA_BASE + portValue * GPIO_OFFSET;
}

static void setCnfAndMode(uintptr_t gpioBaseAddr, uint8_t cnfMode, uint8_t pinValue) {
    uint8_t shamt;

    if (pinValue <= 7u) {
        shamt = pinValue << 2u; //pinValue takes maximum 3 bits + 2 = 5
        GPIO_CRL(gpioBaseAddr) &= ~(0xFu << shamt); //Clear the fields
        GPIO_CRL(gpioBaseAddr) |= static_cast<uint32_t>(cnfMode << shamt); //Set the value
    } else {
        shamt = (pinValue - 8u) << 2u;
        GPIO_CRH(gpioBaseAddr) &= ~(0xFu << shamt);
        GPIO_CRH(gpioBaseAddr) |= static_cast<uint32_t>(cnfMode << shamt);
    }
}

void GPIO::setOutPin(GPIO::Pin pin, GPIO::OutMode mode, GPIO::OutSpeed speed) {
    uint8_t pinValue;
    uintptr_t gpioBaseAddr;

    calcValues(pin, pinValue, gpioBaseAddr);

    uint8_t value = (uint8_t) (static_cast<uint8_t>(mode) << 2u) | static_cast<uint8_t>(speed);
    setCnfAndMode(gpioBaseAddr, value, pinValue);
}

void GPIO::toggle(GPIO::Pin pin) {
    uint8_t pinValue;
    uintptr_t gpioBaseAddr;

    calcValues(pin, pinValue, gpioBaseAddr);

    GPIO_ODR(gpioBaseAddr) ^= (1u << pinValue);
}

void GPIO::digitalWrite(GPIO::Pin pin, bool value) {
    uint8_t pinValue;
    uintptr_t gpioBaseAddr;

    calcValues(pin, pinValue, gpioBaseAddr);

    if (value)
        GPIO_ODR(gpioBaseAddr) |= 1u << pinValue;
    else
        GPIO_ODR(gpioBaseAddr) &= ~(1u << pinValue);
}

void GPIO::setInPin(GPIO::Pin pin, GPIO::InMode mode) {
    uint8_t pinValue;
    uintptr_t gpioBaseAddr;

    calcValues(pin, pinValue, gpioBaseAddr);

    auto value = static_cast<uint8_t>(mode) & 0b1111u;
    setCnfAndMode(gpioBaseAddr, value, pinValue);

    if ((uint8_t) (static_cast<uint8_t>(mode) >> 4u) & 1u)
        GPIO_ODR(gpioBaseAddr) |= 1u << pinValue;
    else
        GPIO_ODR(gpioBaseAddr) &= ~(1u << pinValue);
}

bool GPIO::digitalRead(GPIO::Pin pin) {
    uint8_t pinValue;
    uintptr_t gpioBaseAddr;

    calcValues(pin, pinValue, gpioBaseAddr);

    return (GPIO_IDR(gpioBaseAddr) & (1u << pinValue)) != 0;
}
