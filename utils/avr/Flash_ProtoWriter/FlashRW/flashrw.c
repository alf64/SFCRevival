/*
 * flashrw.c
 *
 *  Created on: 30 wrz 2018
 *      Author: alf64
 */

/*
 * Flash Read-Write functionality implemented here.
 */

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "flashrw_conns.h"
#include "flashrw.h"

void InitFlashRW(void)
{
    // set most pins as outputs
    SI_DDR |= (1<<SI_SER) | (1<<SI_SRCLK) |
            (1<<SI_SRCLR) | (1<<SI_RCLK) |
            (1<<SI_OE);
    FLASHIC_DDR1 |= (1<<FLASHIC_CE) | (1<<FLASHIC_OE);
    SO_DDR |= (1<<SO_SHLD) | (1<<SO_CLK) |
            (1<<SO_CLK_INH) | (1<<SO_CLR);
    FLASHIC_DDR2 |= (1<<FLASHIC_WE);
    SO_DDR |= (1<<SI_OE_DATA) | (1<<DATA_OE) | (1<<DATA_DIR);

    // set serial input data pin as input
    SO_DDR &= ~(1<<SO_QH);
    /*
     * make serial input data pin high impedance (Hi-Z) pin
     * There is no pull-down internal resistor, only pull-ups
     * are present in AVRs.
     *
     * That means by putting '0' when configured as input,
     * we set it to be high impedance pin. This means,
     * the state (value) of the pin is unpredicted when
     * there is nothing connected to it, or something connected
     * is also a high impedance pin.
     *
     * You can just live it, or put an external pull-down resistor.
     */
    SO_PORT &= ~(1<<SO_QH);
}
