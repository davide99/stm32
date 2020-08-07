#include "SPI.h"
#include "GPIO.h"
#include "Utils.h"
#include "common.h"

#define SPI_CR1(base) __RMM(base + 0x0u)
#define SPI_CR1_CPHA     (1u<<0u)
#define SPI_CR1_CPOL     (1u<<1u)
#define SPI_CR1_MSTR     (1u<<2u)
#define SPI_CR1_SPE      (1u<<6u)
#define SPI_CR1_LSBFIRST (1u<<7u)
#define SPI_CR1_SSI      (1u<<8u)
#define SPI_CR1_SSM      (1u<<9u)
#define SPI_CR1_DFF      (1u<<11u)

#define SPI_SR(base)  __RMM(base + 0x8u)
#define SPI_SR_RXNE      (1u<<0u)
#define SPI_SR_TXE       (1u<<1u)
#define SPI_SR_BSY       (1u<<7u)

#define SPI_DR(base)  __RMM(base + 0xCu)


SPI::SPI(SPIn n, GPIO::Pin SSpin) {
    this->SSpin = SSpin;
    this->baseAddr = static_cast<uintptr_t>(n);
    GPIO::setOutPin(SSpin, GPIO::OutMode::PushPull);
    GPIO::digitalWrite(SSpin, true);

    switch (n) {
        case SPIn::SPI1: {
            Utils::enablePeripheral(Utils::Peripheral::PortA);
            GPIO::setOutPin(GPIO::Pin::A5, GPIO::OutMode::AltPushPull); //SCK
            GPIO::setInPin(GPIO::Pin::A6, GPIO::InMode::Floating);      //MISO
            GPIO::setOutPin(GPIO::Pin::A7, GPIO::OutMode::AltPushPull); //MOSI

            Utils::enablePeripheral(Utils::Peripheral::Spi1);
            break;
        }
        case SPIn::SPI2: {
            Utils::enablePeripheral(Utils::Peripheral::PortB);
            GPIO::setOutPin(GPIO::Pin::B13, GPIO::OutMode::AltPushPull); //SCK
            GPIO::setInPin(GPIO::Pin::B14, GPIO::InMode::Floating);      //MISO
            GPIO::setOutPin(GPIO::Pin::B15, GPIO::OutMode::AltPushPull); //MOSI

            Utils::enablePeripheral(Utils::Peripheral::Spi2);
            break;
        }
    }
}

void SPI::beginTransaction(SPImode mode, SPIdiv div, bool format16, bool lsbFirst) const {
    SPI_CR1(this->baseAddr) =
            SPI_CR1_SPE |
            SPI_CR1_MSTR |
            SPI_CR1_SSM |
            SPI_CR1_SSI |
            static_cast<uint8_t>(div) |
            ((mode == SPImode::Mode2 || mode == SPImode::Mode3) ? SPI_CR1_CPOL : 0u) |
            ((mode == SPImode::Mode1 || mode == SPImode::Mode3) ? SPI_CR1_CPHA : 0u) |
            (format16 ? SPI_CR1_DFF : 0u) |
            (lsbFirst ? SPI_CR1_LSBFIRST : 0u);
}

void SPI::endTransaction() const {
    while (!(SPI_SR(this->baseAddr) & SPI_SR_TXE));
    while (SPI_SR(this->baseAddr) & SPI_SR_BSY);
}

uint16_t SPI::transfer(uint16_t data) const {
    SPI_DR(this->baseAddr) = data;
    if (SPI_CR1(this->baseAddr) & SPI_CR1_SPE)
        while (!(SPI_SR(this->baseAddr) & SPI_SR_RXNE));
    return SPI_DR(this->baseAddr);
}

void SPI::slaveSelect() const {
    GPIO::digitalWrite(this->SSpin, false);
}

void SPI::slaveRelease() const {
    GPIO::digitalWrite(this->SSpin, true);
}

SPI::~SPI() {
    SPI_CR1(this->baseAddr) &= ~SPI_CR1_SPE;
}
