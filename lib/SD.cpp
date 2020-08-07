#include "SD.h"

enum command : uint8_t {
    Cmd0 = 0x40u + 0u,
    Cmd1 = 0x40u + 1u,
    Cmd8 = 0x40u + 8u,
    Cmd9 = 0x40u + 9u,
    Cmd10 = 0x40u + 10u,
    Cmd12 = 0x40u + 12u,
    Cmd16 = 0x40u + 16u,
    Cmd17 = 0x40u + 17u,
    Cmd18 = 0x40u + 18u,
    Cmd23 = 0x40u + 23u,
    Cmd24 = 0x40u + 24u,
    Cmd25 = 0x40u + 25u,
    Cmd41 = 0x40u + 41u,
    Cmd55 = 0x40u + 55u,
    Cmd58 = 0x40u + 58u
};

SD::SD(const SPI &spi) : spi(spi) {
    this->spi.slaveRelease();

    for (int i = 0; i < 10; i++)
        this->spi.transfer(0xFFu);

    this->spi.slaveSelect();
    this->spi.beginTransaction();

    //Enter idle state
    this->spi.transfer(Cmd0 | 0x40u);
    this->spi.transfer(0);
    this->spi.transfer(0);
    this->spi.transfer(0);
    this->spi.transfer(0);
    this->spi.transfer(0x95u);

    uint32_t count = 0x1FFFu;
    //Waiting for a response
    while ((this->spi.transfer(0xFFu) != 0x1u) && count)
        count--;

    this->spi.slaveRelease();
    this->spi.transfer(0xFFu);

    this->spi.slaveSelect();

    this->type = 0;
    uint8_t ocr[4];
    if (this->sendCmd(Cmd0, 0) == 0x1u) {
        uint8_t timer1 = 100;

        if (this->sendCmd(Cmd8, 0x1AAu) == 0x1u) {
            //SDC Ver2+
            for (uint8_t &i : ocr)
                i = this->spi.transfer(0xFFu);

            if (ocr[2] == 0x01u && ocr[3] == 0xAAu) {
                do {
                    if (this->sendCmd(Cmd55, 0) <= 0x1u && this->sendCmd(Cmd41, 1u << 30u) == 0x0u)
                        break;
                } while (timer1);

                if (timer1 && this->sendCmd(Cmd58, 0) == 0) {
                    //Check CSS bit
                    for (unsigned char &i : ocr)
                        i = this->spi.transfer(0xFFu);

                    this->type = (ocr[0] & 0x40u) ? 6u : 2u;
                }
            }
        } else {
            //SDC Ver1 or MMC
            type = (this->sendCmd(Cmd55, 0) <= 1u && this->sendCmd(Cmd41, 0) <= 1u) ? 2u : 1u;//SDC:MMC

            do {
                if (type == 2) {
                    if (this->sendCmd(Cmd55, 0) <= 1u && this->sendCmd(Cmd41, 0) == 0)
                        break; //ACMD41
                } else {
                    if (this->sendCmd(Cmd1, 0) == 0)
                        break; //CMD1
                }
            } while (timer1);

            if (!timer1 || this->sendCmd(Cmd16, 512u))
                this->type = 0;
        }
    }

    this->spi.slaveRelease();
    this->spi.transfer(0xFFu);
}

uint8_t SD::sendCmd(uint8_t cmd, uint32_t arg) {
    if (this->readyWait() != 0xFFu)
        return 0xFFu;

    this->spi.transfer(cmd);
    this->spi.transfer((uint8_t) (arg >> 24u));
    this->spi.transfer((uint8_t) (arg >> 16u));
    this->spi.transfer((uint8_t) (arg >> 8u));
    this->spi.transfer((uint8_t) arg);

    uint8_t crc = 0;
    if (cmd == Cmd0)
        crc = 0x95u; //CRC for CMD0
    else if (cmd == Cmd8)
        crc = 0x87u; //CRC for CMD8

    this->spi.transfer(crc);

    if (cmd == Cmd12)
        this->spi.transfer(0xFFu);

    uint8_t n = 10, res;
    do {
        res = this->spi.transfer(0xFFu);
    } while ((res & 0x80u) && --n);

    return res;
}

uint8_t SD::readyWait() {
    uint8_t timer2 = 50u, res;
    this->spi.transfer(0xFFu);

    do {
        res = this->spi.transfer(0xFFu);
    } while ((res != 0xFFu) && --timer2);

    return res;
}

bool SD::read(uint8_t *buff, uint32_t sector) {
    if (!(this->type & 4u))
        sector <<= 9u;

    this->spi.slaveSelect();

    bool ok = (this->sendCmd(Cmd17, sector) == 0) && (this->rxDataBlock(buff, 512u));
    this->spi.slaveRelease();
    this->spi.transfer(0xFFu);

    return ok;
}

bool SD::rxDataBlock(uint8_t *buff, uint32_t btr) {
    uint8_t token;
    uint8_t timer1 = 10;

    do {
        token = this->spi.transfer(0xFFu);
    } while ((token == 0xFFu) && --timer1);

    if (token != 0xFEu)
        return false;

    do {
        *buff++ = this->spi.transfer(0xFFu);
        *buff++ = this->spi.transfer(0xFFu);
    } while (btr -= 2);

    this->spi.transfer(0xFFu);
    this->spi.transfer(0xFFu);

    return true;
}
