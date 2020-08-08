#include "SD.h"

enum Command : uint8_t {
    GoIdleState = 0x40u + 0u,
    SendOpCond = 0x40u + 1u,
    SendIfCond = 0x40u + 8u,
    StopTransmission = 0x40u + 12u,
    SetBlockLen = 0x40u + 16u,
    ReadSingleBlock = 0x40u + 17u,
    ReadMultipleBlock = 0x40u + 18u,
    SendOpCondAcmd = 0x40u + 41u,
    AppCmd = 0x40u + 55u,
    ReadOcr = 0x40u + 58u
};

SD::SD(const SPI &spi) : spi(spi) {
    //Send dummy bytes with CS high before accessing
    for (int i = 0; i < 10; i++)
        this->spi.transfer(0xFFu);

    this->spi.slaveSelect();
    this->spi.beginTransaction();

    //Enter idle state
    this->spi.transfer(GoIdleState);
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
    if (this->sendCmd(GoIdleState, 0) == 0x1u) {
        uint8_t timer1 = 100;

        if (this->sendCmd(SendIfCond, 0x1AAu) == 0x1u) {
            //SDC Ver2+
            for (uint8_t &i : ocr)
                i = this->spi.transfer(0xFFu);

            if (ocr[2] == 0x01u && ocr[3] == 0xAAu) {
                do {
                    if (this->sendCmd(AppCmd, 0) <= 0x1u && this->sendCmd(SendOpCondAcmd, 1u << 30u) == 0x0u)
                        break;
                } while (timer1);

                if (timer1 && this->sendCmd(ReadOcr, 0) == 0) {
                    //Check CSS bit
                    for (unsigned char &i : ocr)
                        i = this->spi.transfer(0xFFu);

                    this->type = (ocr[0] & 0x40u) ? 6u : 2u;
                }
            }
        } else {
            //SDC Ver1 or MMC
            type = (this->sendCmd(AppCmd, 0) <= 1u && this->sendCmd(SendOpCondAcmd, 0) <= 1u) ? 2u : 1u;//SDC:MMC

            do {
                if (type == 2) {
                    if (this->sendCmd(AppCmd, 0) <= 1u && this->sendCmd(SendOpCondAcmd, 0) == 0)
                        break; //ACMD41
                } else {
                    if (this->sendCmd(SendOpCond, 0) == 0)
                        break; //CMD1
                }
            } while (timer1);

            if (!timer1 || this->sendCmd(SetBlockLen, 512u))
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
    if (cmd == GoIdleState)
        crc = 0x95u; //CRC for CMD0
    else if (cmd == SendIfCond)
        crc = 0x87u; //CRC for CMD8

    this->spi.transfer(crc);

    if (cmd == StopTransmission)
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

bool SD::read(uint32_t sector, uint8_t *buff, uint32_t count) {
    if (!(this->type & 4u))
        sector <<= 9u;

    this->spi.slaveSelect();

    if (count == 1) {
        if ((this->sendCmd(ReadSingleBlock, sector) == 0) && (this->rxDataBlock(buff, 512u)))
            count = 0;
    } else {
        if (this->sendCmd(ReadMultipleBlock, sector) == 0) {
            do {
                if (!this->rxDataBlock(buff, 512))
                    break;

                buff += 512;
            } while (--count);

            this->sendCmd(StopTransmission, 0);
        }
    }
    this->spi.slaveRelease();
    this->spi.transfer(0xFFu);

    return count == 0;
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
