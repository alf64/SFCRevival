/*
 * flashrw_conns.h
 *
 *  Created on: 30 wrz 2018
 *      Author: alf64
 */

#ifndef FLASHRW_CONNS_H_
#define FLASHRW_CONNS_H_

/*
 * This file defines connections between
 * AVR pins (FlashRW module) and outside world.
 */

/*
 * ASSUMPTION:
 * All pins of a specific IC are supposed to be
 * connected to a common (the same) port of AVR.
 *
 * If not, please remember to extinguish the ports defines.
 * (i.e. make SI_PORT1 and SI_PORT2 and be aware of the assigned pins)
 */

// ------------ 3x 74HC595 for giving address ----------
#define SI_PORT PORTA
#define SI_DDR DDRA

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
 * Storage Register Output Enable
 *
 * Enables outputs of Storage Register.
 * This means the pins of HC595 will turn
 * form high impedance state to actual data.
 *
 * Active (Enable) on '0'.
 */
#define SI_OE PA4
// -------------------------------------------

// ---------- Flash IC connections -----------
#define FLASHIC_PORT1 PORTA
#define FLASHIC_DDR1 DDRA

/*
 * Flash Chip Enable
 *
 * Enables Flash Chip.
 * Otherwise it stays in standby mode
 * (high-impdenace no matter what's on ROM_OE)
 *
 * Active (Enable) on '0'.
 */
#define FLASHIC_CE PA5

/*
 * ROM Outputs Enable
 *
 * Enables Flash Outputs
 * (Data form a given address are now visible
 * on outputs pins, otherwise pins are high-impedance)
 *
 * Active (Enable) on '0'.
 */
#define FLASHIC_OE PA6


#define FLASHIC_PORT2 PORTD
#define FLASHIC_DDR2 DDRD

/*
 * Flash Write Enable
 *
 * Controls write operations.
 */
#define FLASHIC_WE PD6
// -------------------------------------------

// -------------- 74HC166 ------------
#define SO_PORT PORTC
#define SO_DDR DDRC
#define SO_PIN PINC

/*
 * Shift / Load Enable
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
 * Clock
 */
#define SO_CLK PC6

/*
 * Clock Inhibit (Clock Enable)
 *
 * A logic for forcing chip to ignore clocks
 *
 * '0' Clock enabled
 * '1' Clock disabled (no matter what on Clock,
 * will be interpreted as zero)
 */
#define SO_CLK_INH PC5

/*
 * Clear
 *
 * Resets to zero the internal register.
 * Active (clear) on '0'.
 */
#define SO_CLR PC4

/*
 * Serial Output
 *
 * Data is present here when clocked out from
 * the internal register
 */
#define SO_QH PC3
// ----------------------------------

// ---------- 1x 74HC595 for giving data -----
// this shares port with 74HC166
#define SI_OE_DATA PC2
// -------------------------------------------

// ----------- 74LVC4245 ---------------------
// this shares port with 74HC166
#define DATA_OE PC1
#define DATA_DIR PC0
// -------------------------------------------


#endif /* FLASHRW_CONNS_H_ */
