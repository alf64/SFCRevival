/*
 * sst.c
 *
 *  Created on: 7 maj 2019
 *      Author: bmudlaff
 */

#include "pcbhal.h"
#include "sst.h"


void SSTRet(void)
{
    // clear appropriate pins
    pcbhal_si_ser_clear();
    pcbhal_si_srclk_clear();
    pcbhal_si_rclk_clear();
    pcbhal_so_shld_clear();
    pcbhal_so_clk_clear();
    pcbhal_data_dir_clear();
    pcbhal_so_qh_clear();
    pcbhal_we_5v_clear();

    // set appropriate pins
    pcbhal_si_srclr_set(); // addr and data shift regs out of clear
    pcbhal_si_oe_a_set(); // addr shift regs disable outputs
    pcbhal_si_oe_d_set(); // data shift reg disable outputs
    pcbhal_so_clr_set(); // ls166 out of clear
    pcbhal_data_oe_set(); // 4245 outputs disable
    pcbhal_rom_oe_set(); // mem outputs disable
    pcbhal_rom_ce_set(); // mem chip disable
}

void SSTInit(void)
{
    pcbhal_init();
    SSTRet();
}

sst_ec_t SSTRead(
        uint32_t addr,
        uint8_t* retbt)
{
    if(retbt == NULL)
        return SST_FAILED;

    pcbhal_595_clear();
    pcbhal_166_clear();

    // 1. load addr onto shift regs
    const uint8_t max_addr_bits = 24;
    for(uint8_t i = 0; i < max_addr_bits; i++)
    {
        uint8_t bit = ((uint8_t)(addr>>((max_addr_bits-1)-i))&0x01);
        if(bit == 1)
        {
            pcbhal_si_ser_set();
        }
        else
        {
            pcbhal_si_ser_clear();
        }

        _delay_us(SI_SER_SETUPTIME_US);
        pcbhal_595_sr_single_clock_run();
    }

    // 2. clock data to storage outputs registers
    pcbhal_595_r_single_clock_run();

    // 3. enable address regs outputs
    pcbhal_595a_outs_enable();

    // 4. enable flash chip
    pcbhal_sst_enable();
    _delay_us(SST_SETUPTIME_US); // wait for mem

    // 5. Read data
    pcbhal_4245_set_ba_outs_enable();
    pcbhal_166_enter_loadmode();
    const uint8_t data_bits = 8;
    _delay_us(1.0f);
    pcbhal_166_single_clock_run(); // load data onto 166 internal reg
    pcbhal_166_enter_shiftmode();
    _delay_us(1.0f);
    // gain data
    uint8_t gained_val = 0xBE;
    // tip: QH pin is already showing us what's the MSB (QH) bit
    for(uint8_t i = 0; i < data_bits; i++)
    {
        // read & store bit value
        if(pcbhal_so_qh_get() == 0)
        {
            gained_val &= ~(1<<(7-i));
        }
        else
        {
            gained_val |= (1<<(7-i));
        }

        // shift for the next data
        pcbhal_166_single_clock_run();
    }

    // 6. Return pins to previous state
    pcbhal_si_ser_clear();
    pcbhal_595a_outs_disable();
    pcbhal_sst_disable();
    pcbhal_4245_outs_disable();
    pcbhal_166_enter_loadmode();
    _delay_us(1.0f);

    // 7. Return read data
    *retbt = gained_val;

    return SST_SUCCESS;
}

sst_ec_t SSTWrite(
        uint32_t addr,
        uint8_t writebt)
{
    // TODO: implement here!!!
    return SST_SUCCESS;
}
