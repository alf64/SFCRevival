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

//!< pcb connections between uC and this board
// ----- UC_PORT1 and things connected to it -----
#define UC_PORT1 PORTA
#define UC_DDR1 DDRA
#define UC_PIN1 PINA

/*
 * Serial Data Input
 *
 * This is used for putting data into shift registers.
 */
#define SI_SER PA0

/*
 * Shift Register Clock
 *
 * This is used as a clock for shift registers, when
 * putting the data serially.
 */
#define SI_SRCLK PA1

/*
 * Shift Register Clear
 *
 * Clears the shift registers.
 *
 * Active (Clear) on '0'.
 */
#define SI_SRCLR PA2

/*
 * Storage Register Clock
 *
 * Serves as a clock for copying the data
 * from shift registers to storage registers.
 */
#define SI_RCLK PA3

/*
 * Storage Register Output Enable.
 * Address ICs.
 *
 * Enables outputs of Storage Register.
 * This means the pins of HC595 will turn
 * form high impedance state to actual data.
 *
 * Active (Enable) on '0'.
 */
#define SI_OE_A PA4

/*
 * Storage Register Output Enable
 * Data IC.
 *
 * Enables outputs of Storage Register.
 * This means the pins of HC595 will turn
 * form high impedance state to actual data.
 *
 * Active (Enable) on '0'.
 */
#define SI_OE_D PA5

/*
 * ROM/Flash Chip Enable
 *
 * Enables ROM/Flash Chip.
 * Otherwise it stays in standby mode
 * (high-impdenace no matter what's on ROM_OE)
 *
 * Active (Enable) on '0'.
 */
#define ROM_CE PA6

/*
 * ROM/Flash Chip Outputs Enable
 *
 * Enables ROM/Flash Outputs
 * (Data form a given address are now visible
 * on outputs pins, otherwise pins are high-impedance)
 *
 * Active (Enable) on '0'.
 */
#define ROM_OE PA7
// -------------------------------------------

// ----- UC_PORT2 and things connected to it -----
#define UC_PORT2 PORTC
#define UC_DDR2 DDRC
#define UC_PIN2 PINC

/*
 * Shift / Load Enable for LS166
 *
 * '1' - Shift Enable
 * In this mode, you can clock out the data from
 * the internal register to the serial output
 *
 * '0' - Load Enable
 * In this mode you can copy data from input pins
 * to the internal register
 */
#define SO_SHLD PC7

/*
 * Clock for LS166
 */
#define SO_CLK PC6

/*
 * Clear for LS166
 *
 * Resets to zero the internal register.
 * Active (clear) on '0'.
 */
#define SO_CLR PC5

/*
 * Data direction for 4245
 * '0' : B -> A
 * '1' : A -> B
 */
#define DATA_DIR PC4

/*
 * Output enable for 4245
 * Active (enable) on '0'.
 */
#define DATA_OE PC3

/*
 * Serial Output
 *
 * Data is present here when clocked out from
 * the internal register
 */
#define SO_QH PC2

/*
 * Flash Write Enable
 *
 * Controls write operations.
 */
#define WE_5V PC1

/*
 * Debug button.
 */
#define DBG_BTN PC0
// -------------------------------------------

// ----- time constants -----
/*
 * I'd like to choose shift and load register clock for 74HC595: 5MHz, (period: 0,2us (200ns))
 * but AVR minimum delay (assuming 8MHz clock) is about 375ns.
 * so i am choosing 1 us here
 */
#define SI_SRCLK_PERIOD_US 2.0f
#define SI_SRCLK_HALF_PERIOD_US 1.0f
#define SI_RCLK_PERIOD_US 2.0f
#define SI_RCLK_HALF_PERIOD_US 1.0f
// same goes for 74HC166
#define SO_CLK_PERIOD_US SI_SRCLK_PERIOD_US
#define SO_CLK_HALF_PERIOD_US SI_SRCLK_HALF_PERIOD_US

/*
 * set up time: SI_SER before raising_edge(SI_SRCLK): doc says 30 ns
 * on AVR i am setting this to 1 us
 */
#define SI_SER_SETUPTIME_US 1.0f
// --------------------------

// ----- some other constants -----
//!< Logical '0' means the button is pressed.
#define DBG_BTN_PRESSED 0

//!< Logical '1' means the button is released.
#define DBG_BTN_RELEASED 1

// --------------------------------

#endif /* INCLUDE_BOARDS_MEMI8M_PCB_H_ */
