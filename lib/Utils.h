#ifndef STM32_UTILS_H
#define STM32_UTILS_H

#include <cstdint>
#include <cstddef>

namespace Utils {
    /*
     * 0b100000 means APB1
     * 0b000000 means APB2
     */
    enum class Peripheral : uint8_t {
        AFIO = 0u + 0b000000u,
        PortA = 2u + 0b000000u,
        PortB = 3u + 0b000000u,
        PortC = 4u + 0b000000u,
        Spi1 = 12u + 0b000000u,
        Usart1 = 14u + 0b000000u,
        Spi2 = 14u + 0b100000u,
        Usart2 = 17u + 0b100000u,
        Usart3 = 18u + 0b100000u,
    };

    void enablePeripheral(Peripheral peripheral);

    void disablePeripheral(Peripheral peripheral);

    void reverseString(char *str, size_t len);
}

#endif //STM32_UTILS_H
