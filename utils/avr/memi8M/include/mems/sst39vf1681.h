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

#endif /* INCLUDE_MEMS_SST39VF1681_H_ */
