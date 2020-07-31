#ifndef STM32_GPIO_H
#define STM32_GPIO_H

#include <cstdint>

namespace GPIO {

    enum class Port : uint8_t {
        A = 2u, B, C
    };

    void enablePort(Port port);

    enum class Pin : uint8_t {
        A0 = 0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A15 = 15,
        B0 = 16, B1, B3 = 19, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15,
        C13 = 45, C14, C15
    };

    enum class OutMode : uint8_t {
        PushPull = 0u, OpenDrain, AltPushPull, AltOpenDrain
    };

    enum class OutSpeed : uint8_t {
        TenMHz = 1u, TwoMHz, FiftyMHz
    };

    void setOutPin(Pin pin, OutMode mode = OutMode::PushPull, OutSpeed speed = OutSpeed::TwoMHz);

    enum class InMode : uint8_t {
        Analog = 0u, Floating = 0b0100u, PullDown = 0b01000u, PullUp = 0b11000u
    };

    void setInPin(Pin pin, InMode mode = InMode::Floating);

    void toggle(Pin pin);

    void digitalWrite(Pin pin, bool value);

    bool digitalRead(Pin pin);
}

#endif
