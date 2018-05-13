/*
 * reader.c
 *
 *  Created on: 3 mar 2018
 *      Author: alf64
 */

/*
 * MaskRom reading functionality here
 */

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "reader.h"
#include "connections.h"


void InitReader(void)
{
    // set most pins as outputs
    SI_DDR |= (1<<SI_SER) | (1<<SI_SRCLK) |
            (1<<SI_SRCLR) | (1<<SI_RCLK) |
            (1<<SI_OE);
    ROM_DDR |= (1<<ROM_CE) | (1<<ROM_OE);
    SO_DDR |= (1<<SO_SHLD) | (1<<SO_CLK) |
            (1<<SO_CLK_INH) | (1<<SO_CLR);

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

reader_status_t ResetReader(void)
{
    reader_status_t status = READER_SUCCESS;
    uint8_t gained_val = 0xBE;

    // ----- reset 74HC595's - there 3 of them -----
    SI_PORT |= (1<<SI_OE); // output disable
    SI_PORT &= ~(1<<SI_SRCLR); // Shift register clear
    SI_PORT &= ~(1<<SI_SRCLK); // reset shift reg clock to zero
    SI_PORT &= ~(1<<SI_RCLK); // reset load reg clock to zero
    _delay_us(1.0f);
    SI_PORT |= (1<<SI_SRCLR); // Shift register out-of-clear
    // ---------------------------------------------

    // ----- reset mask rom ------------------------
    ROM_PORT |= (1<<ROM_CE); // chip disabled
    ROM_PORT |= (1<<ROM_OE); // output disabled
    // ---------------------------------------------

    // ----- reset 74HC166 -------------------------
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
        return READER_FAILED;

    // clean up (74HC166)
    SO_PORT &= ~(1<<SO_CLK); // reset clock to zero
    SO_PORT &= ~(1<<SO_CLR); // register clear
    SO_PORT &= ~(1<<SO_SHLD); // enable parallel-load mode
    _delay_us(1.0f);
    SO_PORT |= (1<<SO_CLR); // register out-of-clear
    // ---------------------------------------------

    return status;
}

uint8_t ReadByteReader(uint32_t addr)
{
    uint8_t retval = 0;

    /*
     * SNES mask roms are either with:
     * 1. 20-bit address bus (carts up to 8-Mbit)
     * 2. 23-bit address bus (carts up to 32-Mbit)
     *
     * Assumption: always read 24-bit addr bus
     * (since i have 3x 74HC595 which gives me 3*8 = 24 bit address bus)
     */

    // 1st: shift the addr data in the 595 regs
    const uint8_t no_of_addr_bits = 24;
    const uint32_t raddress = addr;
    for(uint8_t i = 0; i < no_of_addr_bits; i++)
    {
        uint8_t bit = ((uint8_t)(raddress>>((no_of_addr_bits-1)-i))&0x01);
        if(bit == 1)
            SI_PORT |= (1<<SI_SER);
        else
            SI_PORT &= ~(1<<SI_SER);

        _delay_us(SI_SER_SETUPTIME);
        SI_PORT |= (1<<SI_SRCLK);
        _delay_us(SI_CLOCK_HALF_PERIOD);
        SI_PORT &= ~(1<<SI_SRCLK);
        _delay_us(SI_CLOCK_HALF_PERIOD);
    }

    // 2nd: load the addr data onto mask rom
    SI_PORT |= (1<<SI_RCLK);
    _delay_us(SI_CLOCK_HALF_PERIOD);
    SI_PORT &= ~(1<<SI_RCLK);
    _delay_us(SI_CLOCK_HALF_PERIOD);
    SI_PORT &= ~(1<<SI_OE); // 595 regs outputs enable
    ROM_PORT &= ~(1<<ROM_CE); // mask rom chip enable
    ROM_PORT &= ~(1<<ROM_OE); // mask rom output enable
    _delay_us(ROM_SETUPTIME); // mask rom setup time

    // 3rd: read the data from mask rom
    const uint8_t no_of_data_bits = 8;
    // let's read it all!
    uint8_t gained_val = 0xBE;
    // now, assuming the 166 is in parallel-load mode... Load the data to internal 166 reg!
    SO_PORT |= (1<<SO_CLK);
    _delay_us(SO_CLOCK_HALF_PERIOD);
    SO_PORT &= ~(1<<SO_CLK);
    _delay_us(SO_CLOCK_HALF_PERIOD);

    // serial data acquire
    SO_PORT |= (1<<SO_SHLD); // enable shift mode in 74HC166
    _delay_us(1.0f);
    // QH pin is already showing us what's the MSB (QH) bit
    for(uint8_t i = 0; i < no_of_data_bits; i++)
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

        // shift the data
        SO_PORT |= (1<<SO_CLK);
        _delay_us(SO_CLOCK_HALF_PERIOD);
        SO_PORT &= ~(1<<SO_CLK);
        _delay_us(SO_CLOCK_HALF_PERIOD);
    }

    // small clean up
    SI_PORT |= (1<<SI_OE); // 595 regs output disable
    ROM_PORT |= (1<<ROM_CE); // mask rom chip disable
    ROM_PORT |= (1<<ROM_OE); // mask rom output disable
    SO_PORT &= ~(1<<SO_SHLD); // enable parallel-load mode in 74HC166
    _delay_us(1.0f);

    retval = gained_val;

    return retval;
}

