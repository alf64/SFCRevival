/*
 * sst39vf1681.h
 *
 *  Created on: 7 maj 2019
 *      Author: bmudlaff
 */

#ifndef INCLUDE_MEMS_SST39VF1681_H_
#define INCLUDE_MEMS_SST39VF1681_H_

/*
 * Specifics for sst39vf1681 flash chip.
 */

//!< Length (in bytes) of the MEM_DEVICE_NAME (including NULL at the end)
#define MEM_DEVICE_NAME_LEN (11+1)
#define MEM_DEVICE_NAME "SST39VF1681"
//!< Length (in bytes) of the MEM_MANUFACTURER_NAME (including NULL at the end)
#define MEM_MANUFACTURER_NAME_LEN (3+1)
#define MEM_MANUFACTURER_NAME "SST"
#define MEM_MANUFACTURER_ID_ADDRESS 0x0
#define MEM_DEVICE_ID_ADDRESS 0x1
#define MEM_MANUFACTURER_ID 0xBF
#define MEM_DEVICE_ID 0xC8

//!< Capacity of this mem is 2MB (16Mb)
#define MEM_SPACE_CAPACITY 0x00200000

#define MEM_MIN_ADDRESS 0x00000000
#define MEM_MAX_ADDRESS 0x001FFFFF

#define MEM_SECTOR_SIZE 0x1000
#define MEM_BLOCK_SIZE 0x10000

#define MEM_MIN_SECTOR_ADDRESS 0x00000000
#define MEM_MAX_SECTOR_ADDRESS 0x000001FF

#define MEM_MIN_BLOCK_ADDRESS 0x00000000
#define MEM_MAX_BLOCK_ADDRESS 0x0000001F

// ----- time constants -----
/*
 * flash setup time: addr -> data.
 * Doc says: 70 - 90 ns.
 * Setting to 1 us.
 */
#define SST_SETUPTIME_US 1.0f

/*
 * WE pulse width time.
 * Doc says 40ns, setting to 1us.
 */
#define SST_WE_LOW_TIME_US 1.0f

/*
 * WE pulse width high time.
 * Doc says 30ns, setting to 1us.
 */
#define SST_WE_HIGH_TIME_US 1.0f

/*
 * Sector-Erase internal operation time in ms.
 * Doc says it can be up to 25 ms.
 */
#define SST_SECTOR_ERASE_TIME_MS 26

/*
 * Block-Erase internal operation time in ms.
 * Doc says it can be up to 25 ms.
 */
#define SST_BLOCK_ERASE_TIME_MS 26

/*
 * Chip-Erase internal operation time in ms.
 * Doc says it can be up to 50 ms.
 */
#define SST_CHIP_ERASE_TIME_MS 51

/*
 * Byte-Program internal operation time in us.
 * Doc says it can be up to 10us.
 */
#define SST_BYTE_PROGRAM_TIME_US 11
// --------------------------

// ----- commands constants -----
// Command: Byte-Program
#define SST_CMD_BYTE_PROG_WRITE_CYCLES 4
#define SST_CMD_BYTE_PROG_CYCLE1_ADDR 0xAAA
#define SST_CMD_BYTE_PROG_CYCLE1_DATA 0xAA
#define SST_CMD_BYTE_PROG_CYCLE2_ADDR 0x555
#define SST_CMD_BYTE_PROG_CYCLE2_DATA 0x55
#define SST_CMD_BYTE_PROG_CYCLE3_ADDR 0xAAA
#define SST_CMD_BYTE_PROG_CYCLE3_DATA 0xA0
//SST_CMD_BYTE_PROG_CYCLE4_ADDR <- actual addr for byte-prog
//SST_CMD_BYTE_PROG_CYCLE4_DATA <- actual data for byte-prog

// Command: Sector-Erase
#define SST_CMD_SEC_ERASE_WRITE_CYCLES 6
#define SST_CMD_SEC_ERASE_CYCLE1_ADDR 0xAAA
#define SST_CMD_SEC_ERASE_CYCLE1_DATA 0xAA
#define SST_CMD_SEC_ERASE_CYCLE2_ADDR 0x555
#define SST_CMD_SEC_ERASE_CYCLE2_DATA 0x55
#define SST_CMD_SEC_ERASE_CYCLE3_ADDR 0xAAA
#define SST_CMD_SEC_ERASE_CYCLE3_DATA 0x80
#define SST_CMD_SEC_ERASE_CYCLE4_ADDR 0xAAA
#define SST_CMD_SEC_ERASE_CYCLE4_DATA 0xAA
#define SST_CMD_SEC_ERASE_CYCLE5_ADDR 0x555
#define SST_CMD_SEC_ERASE_CYCLE5_DATA 0x55
//SST_CMD_SEC_ERASE_CYCLE6_ADDR <- actual (sector) addr for sec-erase
#define SST_CMD_SEC_ERASE_CYCLE6_DATA 0x50

// Command: Block-Erase
#define SST_CMD_BLOCK_ERASE_WRITE_CYCLES 6
#define SST_CMD_BLOCK_ERASE_CYCLE1_ADDR 0xAAA
#define SST_CMD_BLOCK_ERASE_CYCLE1_DATA 0xAA
#define SST_CMD_BLOCK_ERASE_CYCLE2_ADDR 0x555
#define SST_CMD_BLOCK_ERASE_CYCLE2_DATA 0x55
#define SST_CMD_BLOCK_ERASE_CYCLE3_ADDR 0xAAA
#define SST_CMD_BLOCK_ERASE_CYCLE3_DATA 0x80
#define SST_CMD_BLOCK_ERASE_CYCLE4_ADDR 0xAAA
#define SST_CMD_BLOCK_ERASE_CYCLE4_DATA 0xAA
#define SST_CMD_BLOCK_ERASE_CYCLE5_ADDR 0x555
#define SST_CMD_BLOCK_ERASE_CYCLE5_DATA 0x55
//SST_CMD_BLOCK_ERASE_CYCLE6_ADDR <- actual (block) addr for sec-erase
#define SST_CMD_BLOCK_ERASE_CYCLE6_DATA 0x30

// Command: Chip-Erase
#define SST_CMD_CHIP_ERASE_WRITE_CYCLES 6
#define SST_CMD_CHIP_ERASE_CYCLE1_ADDR 0xAAA
#define SST_CMD_CHIP_ERASE_CYCLE1_DATA 0xAA
#define SST_CMD_CHIP_ERASE_CYCLE2_ADDR 0x555
#define SST_CMD_CHIP_ERASE_CYCLE2_DATA 0x55
#define SST_CMD_CHIP_ERASE_CYCLE3_ADDR 0xAAA
#define SST_CMD_CHIP_ERASE_CYCLE3_DATA 0x80
#define SST_CMD_CHIP_ERASE_CYCLE4_ADDR 0xAAA
#define SST_CMD_CHIP_ERASE_CYCLE4_DATA 0xAA
#define SST_CMD_CHIP_ERASE_CYCLE5_ADDR 0x555
#define SST_CMD_CHIP_ERASE_CYCLE5_DATA 0x55
#define SST_CMD_CHIP_ERASE_CYCLE6_ADDR 0xAAA
#define SST_CMD_CHIP_ERASE_CYCLE6_DATA 0x10

// Command: Software-ID-Entry
#define SST_CMD_SW_ID_ENTRY_WRITE_CYCLES 3
#define SST_CMD_SW_ID_ENTRY_CYCLE1_ADDR 0xAAA
#define SST_CMD_SW_ID_ENTRY_CYCLE1_DATA 0xAA
#define SST_CMD_SW_ID_ENTRY_CYCLE2_ADDR 0x555
#define SST_CMD_SW_ID_ENTRY_CYCLE2_DATA 0x55
#define SST_CMD_SW_ID_ENTRY_CYCLE3_ADDR 0xAAA
#define SST_CMD_SW_ID_ENTRY_CYCLE3_DATA 0x90

// Command: Software-ID-Exit
#define SST_CMD_SW_ID_EXIT_WRITE_CYCLES 1
#define SST_CMD_SW_ID_EXIT_CYCLE1_ADDR 0x00
#define SST_CMD_SW_ID_EXIT_CYCLE1_DATA 0xF0
// --------------------------

#endif /* INCLUDE_MEMS_SST39VF1681_H_ */
