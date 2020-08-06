#ifndef STM32_SERIAL_H
#define STM32_SERIAL_H

#include <cstdint>
#include <string>
#include "Interrupts.h"

enum class SerialN : uint8_t {
    S1, S2, S3
};

class Serial {
private:
    uintptr_t baseAddress;
public:
    explicit Serial(SerialN n, uint32_t baudRate = 9600u, bool enableRxInterrupt = false,
                    Interrupts::Priority priority = Interrupts::Priority::Default);
    void print(char *str) const;
    void print(const std::string& str) const;
    void print(int val, int base = 10) const;
    uint8_t read() const;
    void print(uint8_t byte) const;
    void close() const;
};


#endif
