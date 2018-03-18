/*
 * connections.h
 *
 *  Created on: 3 mar 2018
 *      Author: alf64
 */

/*
 * This file defines connections between
 * AVR pins and outside world.
 */

#ifndef CONNECTIONS_H_
#define CONNECTIONS_H_

/*
 * ASSUMPTION:
 * All pins of a specific IC are supposed to be
 * connected to a common (the same) port of AVR.
 */

// -------------- 74HC595 ------------
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


// -------------- Ninny's Mask ROM ------------
#define ROM_PORT PORTA
#define ROM_DDR DDRA

/*
 * ROM Chip Enable
 *
 * Enables MaskRom Chip.
 * Otherwise it stays in standby mode
 * (high-impdenace no matter what's on ROM_OE)
 *
 * Active (Enable) on '0'.
 */
#define ROM_CE PA5

/*
 * ROM Outputs Enable
 *
 * Enables MaskRom Outputs
 * (Data form a given address are now visible
 * on outputs pins, otherwise pins are high-impedance)
 *
 * Active (Enable) on '0'.
 */
#define ROM_OE PA6
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
 * Resets to zero the intenral register register
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


#endif /* CONNECTIONS_H_ */
