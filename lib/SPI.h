#ifndef STM32_SPI_H
#define STM32_SPI_H

#include <cstdint>
#include "GPIO.h"

enum class SPIn : uintptr_t {
    SPI1 = 0x40013000u, SPI2 = 0x40003800u
};

enum class SPImode {
    Mode0, Mode1, Mode2, Mode3
};

enum class SPIdiv : uint8_t {
    Div2 = 0b000u << 3u,
    Div4 = 0b001u << 3u,
    Div8 = 0b010u << 3u,
    Div16 = 0b011u << 3u,
    Div32 = 0b100u << 3u,
    Div64 = 0b101u << 3u,
    Div128 = 0b110u << 3u,
    Div256 = 0b111u << 3u
};

class SPI {
private:
    GPIO::Pin SSpin;
    uintptr_t baseAddr;
public:
    SPI(SPIn n, GPIO::Pin SSpin);

    void beginTransaction(SPImode mode = SPImode::Mode0, SPIdiv div = SPIdiv::Div64, bool format16 = false,
                          bool lsbFirst = false) const;

    void endTransaction() const;

    uint16_t transfer(uint16_t data) const;

    void slaveSelect() const;

    void slaveRelease() const;

    ~SPI();
};


#endif
