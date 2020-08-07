#ifndef STM32_SD_H
#define STM32_SD_H

#include "SPI.h"
#include <cstdint>

// constants/macros/typdefs
// MMC commands (taken from sandisk MMC reference)
#define MMC_GO_IDLE_STATE			0u		///< initialize card to SPI-type access
#define MMC_SEND_OP_COND			1u		///< set card operational mode
#define MMC_SEND_CSD				9u		///< get card's CSD
#define MMC_SEND_CID				10u		///< get card's CID
#define MMC_SEND_STATUS				13u
#define MMC_SET_BLOCKLEN			16u		///< Set number of bytes to transfer per block
#define MMC_READ_SINGLE_BLOCK		17u		///< read a block
#define MMC_WRITE_BLOCK				24u		///< write a block
#define MMC_PROGRAM_CSD				27u
#define MMC_SET_WRITE_PROT			28u
#define MMC_CLR_WRITE_PROT			29u
#define MMC_SEND_WRITE_PROT			30u
#define MMC_TAG_SECTOR_START		32u
#define MMC_TAG_SECTOR_END			33u
#define MMC_UNTAG_SECTOR			34u
#define MMC_TAG_ERASE_GROUP_START 	35u		///< Sets beginning of erase group (mass erase)
#define MMC_TAG_ERARE_GROUP_END		36u		///< Sets end of erase group (mass erase)
#define MMC_UNTAG_ERASE_GROUP		37u		///< Untag (unset) erase group (mass erase)
#define MMC_ERASE					38u		///< Perform block/mass erase
#define MMC_CRC_ON_OFF				59u		///< Turns CRC check on/off
// R1 Response bit-defines
#define MMC_R1_BUSY					0x80u	///< R1 response: bit indicates card is busy
#define MMC_R1_PARAMETER			0x40u
#define MMC_R1_ADDRESS				0x20u
#define MMC_R1_ERASE_SEQ			0x10u
#define MMC_R1_COM_CRC				0x08u
#define MMC_R1_ILLEGAL_COM			0x04u
#define MMC_R1_ERASE_RESET			0x02u
#define MMC_R1_IDLE_STATE			0x01u
#define MMC_R1_READY_STATE			0x00u
// Data Start tokens
#define MMC_STARTBLOCK_READ			0xFEu	///< when received from card, indicates that a block of data will follow
#define MMC_STARTBLOCK_WRITE		0xFEu	///< when sent to card, indicates that a block of data will follow
#define MMC_STARTBLOCK_MWRITE		0xFCu
// Data Stop tokens
#define MMC_STOPTRAN_WRITE			0xFDu
// Data Error Token values
#define MMC_DE_MASK					0x1Fu
#define MMC_DE_ERROR				0x01u
#define MMC_DE_CC_ERROR				0x02u
#define MMC_DE_ECC_FAIL				0x04u
#define MMC_DE_OUT_OF_RANGE			0x04u
#define MMC_DE_CARD_LOCKED			0x04u
// Data Response Token values
#define MMC_DR_MASK					0x1Fu
#define MMC_DR_ACCEPT				0x05u
#define MMC_DR_REJECT_CRC			0x0Bu
#define MMC_DR_REJECT_WRITE_ERROR	0x0Du


class SD {
private:
    const SPI &spi;
public:
    explicit SD(const SPI &spi);
    bool reset();
    uint8_t sendCommand(uint8_t cmd, uint32_t arg);
    bool read(uint32_t sector, uint8_t* buffer);
    bool write(uint32_t sector, uint8_t* buffer);
    uint8_t command(uint8_t cmd, uint32_t arg);

private:
    uint8_t cardAcmd(uint8_t cmd, uint32_t arg);
};


#endif //STM32_SD_H
