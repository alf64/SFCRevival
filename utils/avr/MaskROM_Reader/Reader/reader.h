/*
 * reader.h
 *
 *  Created on: 3 mar 2018
 *      Author: alf64
 */

#ifndef READER_READER_H_
#define READER_READER_H_


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
// mask rom setup time: addr -> data. Setting to 2 us.
#define ROM_SETUPTIME 2.0f
/*
 * ------
 */

typedef enum{
    READER_SUCCESS,
    READER_FAILED
}reader_status_t;

/*
 * @brief Inits Reader.
 *
 * @details
 * Must be called (once) in order to make reader functions
 * working properly.
 */
void InitReader(void);

/*
 * @brief Resets reader HW.
 *
 * @details
 * This function sets every IC of the reader
 * to some initial/beginning state.
 * This state is as follows:
 * All of serial input shift registers are filled with '0's
 * Parallel input shift register is filled with '0's using its
 * serial input pin (assuming it's connected to the ground).
 *
 * @returns reader_status_t
 * @retval READER_SUCCESS If function succeeded to reset the reader HW.
 * @retval READER_FAILED If function failed.
 */
reader_status_t ResetReader(void);

/*
 * @brief Reads byte from a given address.
 *
 * @param addr An address to read a byte from.
 *
 * @returns Byte read from a given addr.
 */
uint8_t ReadByteReader(uint32_t addr);

#endif /* READER_READER_H_ */
