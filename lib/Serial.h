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
    explicit Serial(SerialN n);
    void print(const char* str);
};


#endif
