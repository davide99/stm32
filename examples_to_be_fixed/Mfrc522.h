#ifndef STM32_MFRC522_H
#define STM32_MFRC522_H

#include "lib/SPI.h"
#include <cstdint>

// A struct used for passing the UID of a PICC.
typedef struct {
    uint8_t size;           // Number of bytes in the UID. 4, 7 or 10.
    uint8_t uidByte[10];
    //uint8_t sak;          // The SAK (Select acknowledge) byte returned from the PICC after successful selection.
} MFRC522_Uid;

class Mfrc522 {
private:
    const SPI &spi;
private:
    void PCD_WriteRegister(uint8_t reg, uint8_t value);

    void PCD_WriteRegister(uint8_t reg, uint8_t count, const uint8_t *values);

    uint8_t PCD_ReadRegister(uint8_t reg);

    void PCD_ReadRegister(uint8_t reg, uint8_t count, uint8_t *values, uint8_t rxAlign);

    void PCD_ClearRegisterBitMask(uint8_t reg, uint8_t mask);

    void PCD_SetRegisterBitMask(uint8_t reg, uint8_t mask);

    void PCD_Reset();

    void PCD_AntennaOn();

    uint8_t
    PCD_TransceiveData(uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t &backLen, uint8_t &validBits,
                       uint8_t rxAlign, int8_t checkCRC);

    uint8_t PCD_CalculateCRC(uint8_t *data, uint8_t length, uint8_t *result);

    uint8_t PCD_CommunicateWithPICC(uint8_t command, uint8_t waitIRq, uint8_t *sendData, uint8_t sendLen,
                                    uint8_t *backData, uint8_t &backLen, uint8_t &validBits, uint8_t rxAlign,
                                    bool checkCRC);

    uint8_t PICC_Select(MFRC522_Uid &uid);


public:
    explicit Mfrc522(const SPI &spi);

    uint8_t getVersion();

    void PCD_EnableIrq();

    bool PICC_ReadCardSerial(MFRC522_Uid &uid);

    uint8_t PICC_HaltA();

    void PCD_ClearInterrupt();

    void PCD_InterruptReactivateReception();
};


#endif
