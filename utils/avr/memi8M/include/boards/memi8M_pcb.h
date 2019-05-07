/*
 * memi8M_pcb.h
 *
 *  Created on: 7 maj 2019
 *      Author: bmudlaff
 */

#ifndef INCLUDE_BOARDS_MEMI8M_PCB_H_
#define INCLUDE_BOARDS_MEMI8M_PCB_H_

/*
 * Define the hw connections and capabilities of the memi8M pcb here.
 */

//!< The board allows for maximum 1MB (8Mb) of the mem capacity
#define BOARD_SPACE_CAPACITY 0x00100000

#define BOARD_MIN_ADDRESS 0x00000000
#define BOARD_MAX_ADDRESS 0x000FFFFF

#endif /* INCLUDE_BOARDS_MEMI8M_PCB_H_ */
