#ifndef STM32_SD_H
#define STM32_SD_H

#include "SPI.h"
#include <cstdint>

class SD {
private:
    const SPI &spi;
    uint8_t type;
public:
    explicit SD(const SPI &spi);
    bool read(uint8_t* buff, uint32_t sector);
private:
    uint8_t sendCmd(uint8_t cmd, uint32_t arg);
    uint8_t readyWait();
    bool rxDataBlock(uint8_t* buff, uint32_t btr);
};


#endif
