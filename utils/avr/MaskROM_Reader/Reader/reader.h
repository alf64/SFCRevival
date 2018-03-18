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
// I choose shift register clock for 74HC595: 5MHz, period is 0,2us (200ns)
#define SI_CLOCK_PERIOD 0.2f
#define SI_CLOCK_HALF_PERIOD 0.1f
// same goes for 74HC166
#define SO_CLOCK_PERIOD SI_CLOCK_PERIOD
#define SO_CLOCK_HALF_PERIOD SI_CLOCK_HALF_PERIOD
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
