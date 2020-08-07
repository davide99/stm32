#include "SD.h"

SD::SD(const SPI &spi) : spi(spi) {
    this->spi.slaveRelease();
}

bool SD::reset() {
    uint8_t retry, r1;
    retry = 0;

    //Send dummy bytes with CS high before accessing
    this->spi.beginTransaction();
    for (uint8_t i = 0; i < 100; i++)
        this->spi.transfer(0xFFu);
    this->spi.endTransaction();


    for(uint8_t i = 255; --i;)
        this->sendCommand(MMC_GO_IDLE_STATE, 0);



    do {
        //Resetting card, go to SPI mode
        r1 = this->sendCommand(MMC_GO_IDLE_STATE, 0);

        retry++;
        if (retry > 100)
            return false;
    } while (r1 != MMC_R1_IDLE_STATE);

    retry = 0;
    do {
        //Initializing card for operation
        r1 = this->sendCommand(MMC_SEND_OP_COND, 0);

        retry++;
        if (retry > 100)
            return false;
    } while (r1 != MMC_R1_READY_STATE);

    //Turn off CRC checking to simplify communication
    this->sendCommand(MMC_CRC_ON_OFF, 0);

    //Set block length to 512 bytes
    this->sendCommand(MMC_SET_BLOCKLEN, 512u);

    return true;
}

uint8_t SD::command(uint8_t cmd, uint32_t arg) {
    uint8_t r1, retry;

    retry = 0;
    //Send command
    this->spi.transfer(cmd | 0x40u);
    this->spi.transfer(arg >> 24u);
    this->spi.transfer(arg >> 16u);
    this->spi.transfer(arg >> 8u);
    this->spi.transfer(arg);
    this->spi.transfer(0x95u); //CRC valid only for MMC_GO_IDLE_STATE

    //End command
    //Wait for response
    //If more than 8 retries, card has timed-out
    //return the received 0xFF

    while ((r1 = this->spi.transfer(0xFFu)) & 0x80u)
        if (retry++ == 254)
            break;

    //return response
    return r1;
}

uint8_t SD::sendCommand(uint8_t cmd, uint32_t arg) {
    uint8_t r1;

    this->spi.slaveSelect();
    this->spi.beginTransaction();
    r1 = command(cmd, arg);
    this->spi.endTransaction();
    this->spi.slaveRelease();

    return r1;
}

bool SD::read(uint32_t sector, uint8_t *buffer) {
    uint8_t r1;

    this->spi.slaveSelect();
    this->spi.beginTransaction();
    r1 = this->command(MMC_READ_SINGLE_BLOCK, sector << 9u);

    //Check for valid response
    if (r1 != MMC_R1_READY_STATE)
        return false;

    //Wait for block start
    while (this->spi.transfer(0xFFu) != MMC_STARTBLOCK_READ);

    //Read in data
    for (uint16_t i = 0; i < 512u; i++)
        *buffer++ = this->spi.transfer(0xFFu);

    //Read 16 bit CRC
    this->spi.transfer(0xFFu);
    this->spi.transfer(0xFFu);

    this->spi.slaveRelease();
    this->spi.endTransaction();
    return true;
}

bool SD::write(uint32_t sector, uint8_t *buffer) {
    uint8_t r1;

    this->spi.beginTransaction();
    r1 = this->command(MMC_WRITE_BLOCK, sector << 9u);

    //Check for valid response
    if (r1)
        return false;

    //Send dummy
    this->spi.transfer(0xFFu);
    //Send data start token
    this->spi.transfer(MMC_STARTBLOCK_WRITE);

    //Write in data
    for (uint16_t i = 0; i < 512u; i++)
        this->spi.transfer(*buffer++);

    //Write 16 bit CRC (dummy values)
    this->spi.transfer(0xFFu);
    this->spi.transfer(0xFFu);

    //Read the data response token
    r1 = this->spi.transfer(0xFFu);
    if ((r1 & MMC_DR_MASK) != MMC_DR_ACCEPT)
        return false;

    //Wait until card not busy
    while (!this->spi.transfer(0xFFu));

    this->spi.endTransaction();
    return true;
}

uint8_t SD::cardAcmd(uint8_t cmd, uint32_t arg) {
    this->sendCommand(0x37u, 0);
    return this->sendCommand(cmd, arg);
}


