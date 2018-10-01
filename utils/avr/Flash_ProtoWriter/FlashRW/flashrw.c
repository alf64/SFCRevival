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

flashrw_status_t ResetFlashRW(void)
{
    // ----- reset 74HC595's - there 3 + 1 of them -----
    SI_PORT |= (1<<SI_OE); // output disable (3x 74HC595)
    SO_PORT |= (1<<SI_OE_DATA); // output disable (1x 74HC595)
    SI_PORT &= ~(1<<SI_SRCLR); // Shift register clear
    SI_PORT &= ~(1<<SI_SRCLK); // reset shift reg clock to zero
    SI_PORT &= ~(1<<SI_RCLK); // reset load reg clock to zero
    _delay_us(1.0f);
    SI_PORT |= (1<<SI_SRCLR); // Shift register out-of-clear
    // ---------------------------------------------

    // ----- reset flash ic ------------------------
    FLASHIC_PORT1 |= (1<<FLASHIC_CE); // chip disabled
    FLASHIC_PORT1 |= (1<<FLASHIC_OE); // output disabled
    FLASHIC_PORT2 |= (1<<FLASHIC_WE); // write disable
    // ---------------------------------------------

    // ----- reset 74LVC4245 -----------------------
    SO_PORT &= ~(1<<DATA_DIR); // direction: B->A
    SO_PORT |= (1<<DATA_OE); // isolation
    // ---------------------------------------------

    // ----- reset & test 74HC166 -------------------
    SO_PORT &= ~(1<<SO_CLK); // reset clock to zero
    SO_PORT |= (1<<SO_SHLD); // enable serial-in mode
    SO_PORT &= ~(1<<SO_CLR); // register clear
    SO_PORT &= ~(1<<SO_CLK_INH); // clock allow
    _delay_us(1.0f);
    SO_PORT |= (1<<SO_CLR); // register out-of-clear

    // now let's stuff the register with default value,
    // depending on whatever is connected to SER pin
    for(uint8_t i = 0; i < 8; i++)
    {
        _delay_us(SO_CLOCK_HALF_PERIOD);
        SO_PORT &= ~(1<<SO_CLK);
        _delay_us(SO_CLOCK_HALF_PERIOD);
        SO_PORT |= (1<<SO_CLK);
    }

    // now the register shall be filled with '0's or '1's
    // QH pin is already showing us what's the MSB (QH) bit
    // let's read it all!
    uint8_t gained_val = 0xBE;
    for(uint8_t i = 0; i < 8; i++)
    {
        uint8_t pin_read = ((SO_PIN>>SO_QH)&0x01);
        if(pin_read == 0)
        {
            gained_val &= ~(1<<(7-i));
        }
        else
        {
            gained_val |= (1<<(7-i));
        }

        SO_PORT &= ~(1<<SO_CLK);
        _delay_us(SO_CLOCK_HALF_PERIOD);
        SO_PORT |= (1<<SO_CLK);
        _delay_us(SO_CLOCK_HALF_PERIOD);
    }

    // now we shall have gained_val either 0x0 or 0xFF
    if( (gained_val != 0x0) && (gained_val != 0xFF) )
        return FLASHRW_FAILED;

    // clean up (74HC166)
    SO_PORT &= ~(1<<SO_CLK); // reset clock to zero
    SO_PORT &= ~(1<<SO_CLR); // register clear
    SO_PORT &= ~(1<<SO_SHLD); // enable parallel-load mode
    _delay_us(1.0f);
    SO_PORT |= (1<<SO_CLR); // register out-of-clear
    // ---------------------------------------------

    return FLASHRW_SUCCESS;
}
