#ifndef STM32_SERIAL_H
#define STM32_SERIAL_H

#include <cstdint>

enum class SerialN : uint8_t {
    S1, S2, S3
};

class Serial {
private:
    SerialN n;
public:
    explicit Serial(SerialN n, uint32_t baudRate = 9600u);
    uint8_t readByte();
    void printByte(uint8_t byte);
};


#endif
