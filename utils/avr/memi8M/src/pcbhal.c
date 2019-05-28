/*
 * pcbhal.c
 *
 *  Created on: 27 maj 2019
 *      Author: bmudlaff
 */

#include "pcbhal.h"

/*
 * HAL functions here for pcb controls.
 */

void pcbhal_init()
{
    // set pins as outputs
    UC_DDR1 = ((1<<SI_SER)|(1<<SI_SRCLK)|(1<<SI_SRCLR)|(1<<SI_RCLK)|
            (SI_OE_A)|(1<<SI_OE_D)|(1<<ROM_CE)|(1<<ROM_OE));
    UC_DDR2 = ((1<<SO_SHLD)|(1<<SO_CLK)|(1<<SO_CLR)|(1<<DATA_DIR)|
            (1<<DATA_OE)|(1<<WE_5V));
    // set pins as inputs
    UC_DDR2 &= ~(1<<SO_QH);
}

