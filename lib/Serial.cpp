#include "Serial.h"

#ifndef F_CPU
#define F_CPU 8000000ul
#warning Assuming F_CPU = 8 MHz
#endif

Serial::Serial(SerialN n) {
    this->n = n;

}

void Serial::print(const char *str) {

}

