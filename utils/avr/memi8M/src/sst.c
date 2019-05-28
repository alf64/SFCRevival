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

void SSTInit(void)
{
    pcbhal_init();

    // assumption: all avr uc i/o pins are by default '0'

    pcbhal_si_srclr_set(); // addr and data shift regs out of clear
    pcbhal_si_oe_a_set(); // addr shift regs disable outputs
    pcbhal_si_oe_d_set(); // data shift reg disable outputs
    pcbhal_so_clr_set(); // ls166 out of clear
    pcbhal_data_oe_set(); // 4245 outputs disable
    pcbhal_rom_oe_set(); // mem outputs disable
    pcbhal_rom_ce_set(); // mem chip disable
}
