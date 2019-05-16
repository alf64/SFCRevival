/*
 * memi8M_pcb.h
 *
 *  Created on: 7 maj 2019
 *      Author: bmudlaff
 */

#ifndef INCLUDE_BOARDS_MEMI8M_PCB_H_
#define INCLUDE_BOARDS_MEMI8M_PCB_H_

#include "mems/sst39vf1681.h"

/*
 * The memi8M board allows for maximum 1MB (8Mb) of the mem capacity,
 * since A20 pin is connected to GND (not used).
 * That's why the capabilities of the mem are reduced.
 */

#define BOARD_SPACE_CAPACITY 0x00100000

#define BOARD_MIN_ADDRESS 0x00000000
#define BOARD_MAX_ADDRESS 0x000FFFFF

#define BOARD_MIN_SECTOR_ADDRESS 0x00000000
#define BOARD_MAX_SECTOR_ADDRESS 0x000000FF

#define BOARD_MIN_BLOCK_ADDRESS 0x00000000
#define BOARD_MAX_BLOCK_ADDRESS 0x0000000F


#endif /* INCLUDE_BOARDS_MEMI8M_PCB_H_ */
