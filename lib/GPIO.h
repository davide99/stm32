#ifndef STM32_GPIO_H
#define STM32_GPIO_H

#include <cstdint>

namespace GPIO {
    enum class Port : uint8_t {
        A, B, C
    };

    enum class Pin : uint8_t {
        A0 = 0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A15 = 15,
        B0 = 16, B1, B3 = 19, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15,
        C13 = 45, C14, C15
    };

    enum class Mode : uint8_t {
        IN_ANALOG = 0b0000u, IN_FLOATING = 0b0100u, IN_PULL = 0b1000u,
        OUT_PUSH_PULL = 0b00u, OUT_OPEN_DRAIN = 0b01u
    };

    enum class OutSpeed : uint8_t {
        O_10MHZ = 0b01u, O_2MHZ = 0b10u, O_50MHZ = 0b11u
    };

    void enablePort(Port port);

    void pinMode(Pin pin, Mode mode, OutSpeed = OutSpeed::O_2MHZ);

    bool digitalRead(Pin pin);

    void toggle(Pin pin);
}

#endif
