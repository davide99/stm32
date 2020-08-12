#ifndef STM32_CLOCK_H
#define STM32_CLOCK_H

#include <cstdint>

namespace Clock {
    uint8_t getSystemClockMHz();
    uint8_t getAHBClockMHz();
};


#endif //STM32_CLOCK_H
