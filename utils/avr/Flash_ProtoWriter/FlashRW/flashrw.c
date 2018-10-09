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

uint8_t ReadByteFlashRW(uint32_t addr)
{
    uint8_t retval = 0;

    /*
     * There are 4 74HC595 ICs in a chain.
     * 3 for address (3 * 8 bit) and 1 for data (8 bit).
     * Even if only 24 bits are for the valid address, we need to put
     * all 32 bits into the chain, to fill all the ICs. This ensures that we always
     * load all the chain (prevents from bugs).
     */

    // 1st: shift the addr data in the 595 regs
    const uint8_t no_of_addr_bits = 32;
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

    // 2nd: load the addr data onto flash ic
    SI_PORT |= (1<<SI_RCLK);
    _delay_us(SI_CLOCK_HALF_PERIOD);
    SI_PORT &= ~(1<<SI_RCLK);
    _delay_us(SI_CLOCK_HALF_PERIOD);
    SI_PORT &= ~(1<<SI_OE); // 3x 74HC595 regs outputs enable
    FLASHIC_PORT1 &= ~(1<<FLASHIC_CE); // flashic chip enabled
    FLASHIC_PORT1 &= ~(1<<FLASHIC_OE); // flashic output enabled
    SO_PORT &= ~(1<<DATA_DIR); // 74lvc4245 direction: B->A
    SO_PORT &= ~(1<<DATA_OE); // 74lvc4245 data flow enable
    _delay_us(FLASHIC_SETUPTIME); // flash ic setup time

    // 3rd: read the data from flash ic
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

    //small clean up
    SI_PORT |= (1<<SI_OE); // 595 regs output disable
    FLASHIC_PORT1 |= (1<<FLASHIC_CE); // flash chip disable
    FLASHIC_PORT1 |= (1<<FLASHIC_OE); // flash chip output disable
    SO_PORT |= (1<<DATA_OE); // 74lvc4245 isolation
    SO_PORT &= ~(1<<SO_SHLD); // enable parallel-load mode in 74HC166
    _delay_us(1.0f);

    retval = gained_val;

    return retval;
}

/*
 * Shifts in given:
 * (max 24 bit) addr and (max 8bit) data into 74HC595 chain
 * and turns on registers outputs.
 *
 * @param data_addr
 * This is uint32_t variable from which:
 *  a) the MSB 8-bit is treated as data to be loaded into flash ic
 *  b) the rest 24-bit is treated as addr for the data to fall into flash ic
 */
void ProvideAddrAndDataFlashRW(
        uint32_t data_addr)
{
    // 1st: shift the addr data in the 595 regs
    const uint8_t no_of_addr_bits = 32;
    const uint32_t raddress = data_addr;
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

    // 2nd: load the addr data onto flash ic
    SI_PORT |= (1<<SI_RCLK);
    _delay_us(SI_CLOCK_HALF_PERIOD);
    SI_PORT &= ~(1<<SI_RCLK);
    _delay_us(SI_CLOCK_HALF_PERIOD);
    SI_PORT &= ~(1<<SI_OE); // 3x 74HC595 regs (with addr) outputs enable
    SO_PORT &= ~(1<<SI_OE_DATA); // 1x 74HC595 reg (with data) output enable
}

void WriteByteFlashRW(
        uint8_t data,
        uint32_t address)
{
    // zero out the MSB 8-bit, since these are for data
    uint32_t addr = address & (~((uint32_t)0xFF000000));
    // concatenate data & addr
    uint32_t dataaddr = (((uint32_t)data)<<24) | addr;

    // prepare FLASH IC for WE# controlled Byte-Program
    FLASHIC_PORT1 |= (1<<FLASHIC_OE); // Outputs disable
    FLASHIC_PORT2 |= (1<<FLASHIC_WE); // hold WE high
    FLASHIC_PORT1 &= ~(1<<FLASHIC_CE); // Chip enable

    // prepare 74lvc4245
    SO_PORT |= (1<<DATA_DIR); // 74lvc4245 direction: A->B
    SO_PORT &= ~(1<<DATA_OE); // 74lvc4245 data flow enable

    // provide 0xAA data onto 0xAAA addr
    ProvideAddrAndDataFlashRW((uint32_t)0xAA000AAA);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x55 data onto 0x555 addr
    ProvideAddrAndDataFlashRW((uint32_t)0x55000555);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0xA0 data onto 0xAAA addr
    ProvideAddrAndDataFlashRW((uint32_t)0xA0000AAA);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide data onto the addr
    ProvideAddrAndDataFlashRW(dataaddr);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // wait for the internal Program operation to complete
    _delay_us(FLASHIC_BYTE_PROGRAM_TIME);

    // clean things up
    FLASHIC_PORT1 |= (1<<FLASHIC_CE); // flash ic chip disable
    SI_PORT |= (1<<SI_OE); // 3x 74HC595 regs (with addr) outputs disable
    SO_PORT |= (1<<SI_OE_DATA); // 1x 74HC595 reg (with data) output disable
    SO_PORT &= ~(1<<DATA_DIR); // 74lvc4245 direction: B->A
    SO_PORT |= (1<<DATA_OE); // 74lvc4245 isolation
}

void SectorEraseFlashRW(uint32_t addr)
{
    uint32_t sector_addr = addr & (~((uint32_t)0xFFF)); // zero out the 4096-1 part
    sector_addr = sector_addr & (~((uint32_t)0xFF000000)); // zero out the part for data

    // prepare FLASH IC for WE# controlled Sector-Erase
    FLASHIC_PORT1 |= (1<<FLASHIC_OE); // Outputs disable
    FLASHIC_PORT2 |= (1<<FLASHIC_WE); // hold WE high
    FLASHIC_PORT1 &= ~(1<<FLASHIC_CE); // Chip enable

    // prepare 74lvc4245
    SO_PORT |= (1<<DATA_DIR); // 74lvc4245 direction: A->B
    SO_PORT &= ~(1<<DATA_OE); // 74lvc4245 data flow enable

    // provide 0xAA data onto 0xAAA addr
    ProvideAddrAndDataFlashRW((uint32_t)0xAA000AAA);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x55 data onto 0x555 addr
    ProvideAddrAndDataFlashRW((uint32_t)0x55000555);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x80 data onto 0xAAA addr
    ProvideAddrAndDataFlashRW((uint32_t)0x80000AAA);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0xAA data onto 0xAAA addr
    ProvideAddrAndDataFlashRW((uint32_t)0xAA000AAA);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x55 data onto 0x555 addr
    ProvideAddrAndDataFlashRW((uint32_t)0x55000555);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x50 data onto sector_addr
    uint32_t data_saddr = (((uint32_t)(0x50))<<24) | sector_addr;
    ProvideAddrAndDataFlashRW(data_saddr);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // wait for the internal Sector-Erase Operation to complete
    _delay_ms(FLASHIC_SECTOR_ERASE_TIME);

    // clean things up
    FLASHIC_PORT1 |= (1<<FLASHIC_CE); // flash ic chip disable
    SI_PORT |= (1<<SI_OE); // 3x 74HC595 regs (with addr) outputs disable
    SO_PORT |= (1<<SI_OE_DATA); // 1x 74HC595 reg (with data) output disable
    SO_PORT &= ~(1<<DATA_DIR); // 74lvc4245 direction: B->A
    SO_PORT |= (1<<DATA_OE); // 74lvc4245 isolation
}

void BlockEraseFlashRW(uint32_t addr)
{
    uint32_t block_addr = addr & (~((uint32_t)0xFFFF)); // zero out the 65536-1 part
    block_addr = block_addr & (~((uint32_t)0xFF000000)); // zero out the part for data

    // prepare FLASH IC for WE# controlled Block-Erase
    FLASHIC_PORT1 |= (1<<FLASHIC_OE); // Outputs disable
    FLASHIC_PORT2 |= (1<<FLASHIC_WE); // hold WE high
    FLASHIC_PORT1 &= ~(1<<FLASHIC_CE); // Chip enable

    // prepare 74lvc4245
    SO_PORT |= (1<<DATA_DIR); // 74lvc4245 direction: A->B
    SO_PORT &= ~(1<<DATA_OE); // 74lvc4245 data flow enable

    // provide 0xAA data onto 0xAAA addr
    ProvideAddrAndDataFlashRW((uint32_t)0xAA000AAA);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x55 data onto 0x555 addr
    ProvideAddrAndDataFlashRW((uint32_t)0x55000555);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x80 data onto 0xAAA addr
    ProvideAddrAndDataFlashRW((uint32_t)0x80000AAA);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0xAA data onto 0xAAA addr
    ProvideAddrAndDataFlashRW((uint32_t)0xAA000AAA);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x55 data onto 0x555 addr
    ProvideAddrAndDataFlashRW((uint32_t)0x55000555);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x30 data onto block_addr
    uint32_t data_baddr = (((uint32_t)(0x30))<<24) | block_addr;
    ProvideAddrAndDataFlashRW(data_baddr);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // wait for the internal Block-Erase Operation to complete
    _delay_ms(FLASHIC_BLOCK_ERASE_TIME);

    // clean things up
    FLASHIC_PORT1 |= (1<<FLASHIC_CE); // flash ic chip disable
    SI_PORT |= (1<<SI_OE); // 3x 74HC595 regs (with addr) outputs disable
    SO_PORT |= (1<<SI_OE_DATA); // 1x 74HC595 reg (with data) output disable
    SO_PORT &= ~(1<<DATA_DIR); // 74lvc4245 direction: B->A
    SO_PORT |= (1<<DATA_OE); // 74lvc4245 isolation
}

void ChipEraseFlashRW(void)
{
    // prepare FLASH IC for WE# controlled Sector-Erase
    FLASHIC_PORT1 |= (1<<FLASHIC_OE); // Outputs disable
    FLASHIC_PORT2 |= (1<<FLASHIC_WE); // hold WE high
    FLASHIC_PORT1 &= ~(1<<FLASHIC_CE); // Chip enable

    // prepare 74lvc4245
    SO_PORT |= (1<<DATA_DIR); // 74lvc4245 direction: A->B
    SO_PORT &= ~(1<<DATA_OE); // 74lvc4245 data flow enable

    // provide 0xAA data onto 0xAAA addr
    ProvideAddrAndDataFlashRW((uint32_t)0xAA000AAA);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x55 data onto 0x555 addr
    ProvideAddrAndDataFlashRW((uint32_t)0x55000555);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x80 data onto 0xAAA addr
    ProvideAddrAndDataFlashRW((uint32_t)0x80000AAA);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0xAA data onto 0xAAA addr
    ProvideAddrAndDataFlashRW((uint32_t)0xAA000AAA);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x55 data onto 0x555 addr
    ProvideAddrAndDataFlashRW((uint32_t)0x55000555);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // provide 0x10 data onto 0xAAA addr
    ProvideAddrAndDataFlashRW((uint32_t)0x10000AAA);
    FLASHIC_PORT2 &= ~(1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_LOW_TIME);
    FLASHIC_PORT2 |= (1<<FLASHIC_WE);
    _delay_us(FLASHIC_WE_HIGH_TIME);

    // wait for the internal Sector-Erase Operation to complete
    _delay_ms(FLASHIC_CHIP_ERASE_TIME);

    // clean things up
    FLASHIC_PORT1 |= (1<<FLASHIC_CE); // flash ic chip disable
    SI_PORT |= (1<<SI_OE); // 3x 74HC595 regs (with addr) outputs disable
    SO_PORT |= (1<<SI_OE_DATA); // 1x 74HC595 reg (with data) output disable
    SO_PORT &= ~(1<<DATA_DIR); // 74lvc4245 direction: B->A
    SO_PORT |= (1<<DATA_OE); // 74lvc4245 isolation
}
