/*
 * flashrw.h
 *
 *  Created on: 30 wrz 2018
 *      Author: alf64
 */

#ifndef FLASHRW_FLASHRW_H_
#define FLASHRW_FLASHRW_H_

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

/*
 * flash setup time: addr -> data.
 * Doc says: 70 - 90 ns.
 * Setting to 2 us.
 */
#define FLASH_SETUPTIME 2.0f
// --------------------------

typedef enum{
    FLASHRW_SUCCESS,
    FLASHRW_FAILED
}flashrw_status_t;

/*
 * @brief Inits Reader.
 *
 * @details
 * Must be called (once) in order to make flashrw functions
 * working properly.
 */
void InitFlashRW(void);

#endif /* FLASHRW_FLASHRW_H_ */