#include "Utils.h"
#include "common.h"

#define RCC_APB2ENR_ADDR (0x40021000u + 0x18u)
//APB1ENR is @ RCC_APB2ENR_ADDR + 4

void Utils::enablePeripheral(Utils::Peripheral peripheral) {
    auto p = static_cast<uint8_t>(peripheral);

    __RMM(RCC_APB2ENR_ADDR + ((uint8_t) (p >> 3u) & ~0b11u)) |= (1u << (p & 0b11111u));
}

void Utils::disablePeripheral(Utils::Peripheral peripheral) {
    auto p = static_cast<uint8_t>(peripheral);

    __RMM(RCC_APB2ENR_ADDR + ((uint8_t) (p >> 3u) & ~0b11u)) &= ~(1u << (p & 0b11111u));
}

void Utils::reverseString(char *str, size_t len) {
    char *right = str + len - 1;
    char tmp;

    while (str < right) {
        tmp = *str;
        *(str++) = *right;
        *(right--) = tmp;
    }
}
