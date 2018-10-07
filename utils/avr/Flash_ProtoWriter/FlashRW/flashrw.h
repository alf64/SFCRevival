/*
 * flashrw.h
 *
 *  Created on: 30 wrz 2018
 *      Author: alf64
 */

#ifndef FLASHRW_FLASHRW_H_
#define FLASHRW_FLASHRW_H_

/*
 * ----- Settings -----
 */
// I'd like to choose shift and load register clock for 74HC595: 5MHz, (period: 0,2us (200ns))
// but AVR minimum delay (assuming 8MHz clock) is about 375ns.
// so i am choosing 1 us here
#define SI_CLOCK_PERIOD 2.0f
#define SI_CLOCK_HALF_PERIOD 1.0f
// same goes for 74HC166
#define SO_CLOCK_PERIOD SI_CLOCK_PERIOD
#define SO_CLOCK_HALF_PERIOD SI_CLOCK_HALF_PERIOD

// set up time: SI_SER before raising_edge(SI_SRCLK): doc says 30 ns
// on AVR i am setting this to 1 us
#define SI_SER_SETUPTIME 1.0f

/*
 * flash setup time: addr -> data.
 * Doc says: 70 - 90 ns.
 * Setting to 2 us.
 */
#define FLASHIC_SETUPTIME 2.0f

/*
 * WE pulse width time.
 * Doc says 40ns, setting to 1us.
 */
#define FLASHIC_WE_LOW_TIME 1.0f

/*
 * WE pulse width high time.
 * Doc says 30ns, setting to 1us.
 */
#define FLASHIC_WE_HIGH_TIME 1.0f

/*
 * Sector-Erase internal operation time in ms.
 * Doc says it can be up to 25 ms.
 */
#define FLASHIC_SECTOR_ERASE_TIME 26

/*
 * Block-Erase internal operation time in ms.
 * Doc says it can be up to 25 ms.
 */
#define FLASHIC_BLOCK_ERASE_TIME 26

/*
 * Chip-Erase internal operation time in ms.
 * Doc says it can be up to 50 ms.
 */
#define FLASHIC_CHIP_ERASE_TIME 51

/*
 * Byte-Program internal operation time in us.
 * Doc says it can be up to 10us.
 */
#define FLASHIC_BYTE_PROGRAM_TIME 11
// --------------------------

typedef enum{
    FLASHRW_SUCCESS,
    FLASHRW_FAILED
}flashrw_status_t;

/*
 * @brief Inits Reader.
 *
 * @details
 * Must be called (once) in order to make flashrw functions
 * working properly.
 */
void InitFlashRW(void);

/*
 * @brief Resets reader HW.
 *
 * @details
 * This function sets every IC of the flashrw
 * to some initial/beginning state.
 * This state is as follows:
 * All of serial input shift registers are filled with '0's.
 * These registers outputs are disabled.
 * Parallel input shift register is filled with '0's using its
 * serial input pin (assuming it's connected to the ground).
 * FLASH IC is turned into read mode and its outputs are disabled.
 *
 * @returns flashrw_status_t
 * @retval FLASHRW_SUCCESS If function succeeded to reset the reader HW.
 * @retval FLASHRW_FAILED If function failed.
 */
flashrw_status_t ResetFlashRW(void);

/*
 * @brief Reads byte from a given address.
 *
 * @param addr An address to read a byte from.
 *
 * @returns Byte read from a given addr.
 */
uint8_t ReadByteFlashRW(uint32_t addr);

/*
 * @brief Writes data (8-bit) to the flash ic.
 *
 * @param data A byte to be written into flash ic.
 * @param addr An address (max 24 bit) where the data shall be written.
 */
void WriteByteFlashRW(
        uint8_t data,
        uint32_t address);

/*
 * @brief Erases sector of memory from FLASH IC.
 *
 * @details
 * Assuming sector size is 4096 (0x1000) bytes,
 * so the given addr may be anything and
 * the sector address will be obtained by performing
 * & ~(0xFFF)
 *
 * Example:
 *  - you are giving addr = 0x01
 *  - obtained sector address will be: 0x00
 *  - entire sector (addresses: 0x00 - 0xFFF will be erased)
 *
 * @param addr
 * The address of a byte. The sector address of a given address
 * will be obtained and this sector will be erased.
 */
void SectorEraseFlashRW(uint32_t addr);

/*
 * @brief Erases block of memory from FLASH IC.
 *
 * Assuming block size is 64kB = 65536 (0x10000) bytes,
 * so the given addr may be anything and
 * the block address will be obtained by performing
 * & ~(0xFFFF)
 *
 * Example:
 *  - you are giving addr = 0x1FFFFF
 *  - obtained block address will be: 0x1F
 *  - entire block (addresses: 0x1F0000 - 0x1FFFFF will be erased)
 *
 * @param addr
 * The address of a byte. The block address of a given address
 * will be obtained and this block will be erased.
 */
void BlockEraseFlashRW(uint32_t addr);

/*
 * @bried Erases the whole FLASH IC.
 */
void ChipEraseFlashRW(void);

#endif /* FLASHRW_FLASHRW_H_ */
