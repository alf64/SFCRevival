/*
 * sst.c
 *
 *  Created on: 7 maj 2019
 *      Author: bmudlaff
 */

#include "pcbhal.h"
#include "sst.h"

/*
 * Read, write, erase functionality here.
 */

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

    if((addr > BOARD_MAX_ADDRESS) || (addr < BOARD_MIN_ADDRESS))
        return SST_INVALID_INPUT;

    // TODO:
    // 1. load the addr onto shift registers
    // 2. move data from shift registers to storage output registers
    // 3. enable outputs
    // 4. enable flash ic
    // 5. read data from ic and store it to retbt
    // 6. return pins to previous state

    return SST_SUCCESS;
}
