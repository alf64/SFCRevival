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

#define MEM_CHIP_NAME "SST39VF1681"

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

/*
 * Specific programming and interfacing codes here
 */

#endif /* INCLUDE_MEMS_SST39VF1681_H_ */
