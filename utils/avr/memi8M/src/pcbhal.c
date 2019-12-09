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
            (1<<SI_OE_A)|(1<<SI_OE_D)|(1<<ROM_CE)|(1<<ROM_OE));
    UC_DDR2 = ((1<<SO_SHLD)|(1<<SO_CLK)|(1<<SO_CLR)|(1<<DATA_DIR)|
            (1<<DATA_OE)|(1<<WE_5V));
    // set pins as inputs
    UC_DDR2 &= ~(1<<SO_QH);
}

void pcbhal_4245_set_ab_outs_enable()
{
    pcbhal_data_dir_set();
    pcbhal_data_oe_clear();
}

void pcbhal_4245_set_ba_outs_enable()
{
    pcbhal_data_dir_clear();
    pcbhal_data_oe_clear();
}

void pcbhal_4245_outs_disable()
{
    pcbhal_data_oe_set();
}

void pcbhal_595a_outs_enable()
{
    pcbhal_si_oe_a_clear();
}

void pcbhal_595a_outs_disable()
{
    pcbhal_si_oe_a_set();
}

void pcbhal_595d_outs_enable()
{
    pcbhal_si_oe_d_clear();
}

void pcbhal_595d_outs_disable()
{
    pcbhal_si_oe_d_set();
}

void pcbhal_595_clear()
{
    _delay_us(1.0f);
    pcbhal_si_srclr_toggle();
    _delay_us(1.0f);
    pcbhal_si_srclr_toggle();
}

void pcbhal_595_sr_single_clock_run()
{
    _delay_us(SI_SRCLK_HALF_PERIOD_US);
    pcbhal_si_srclk_toggle();
    _delay_us(SI_SRCLK_HALF_PERIOD_US);
    pcbhal_si_srclk_toggle();
}

void pcbhal_595_r_single_clock_run()
{
    _delay_us(SI_RCLK_HALF_PERIOD_US);
    pcbhal_si_rclk_toggle();
    _delay_us(SI_RCLK_HALF_PERIOD_US);
    pcbhal_si_rclk_toggle();
}

void pcbhal_sst_chip_enable()
{
    pcbhal_rom_ce_clear();
}

void pcbhal_sst_chip_disable()
{
    pcbhal_rom_ce_set();
}

void pcbhal_sst_outs_enable()
{
    pcbhal_rom_oe_clear();
}

void pcbhal_sst_outs_disable()
{
    pcbhal_rom_oe_set();
}

void pcbhal_sst_write_enable()
{
    pcbhal_we_5v_clear();
}

void pcbhal_sst_write_disable()
{
    pcbhal_we_5v_set();
}

void pcbhal_sst_w_single_clock_run()
{
    pcbhal_we_5v_toggle();
    _delay_us(SST_WE_LOW_TIME_US);
    pcbhal_we_5v_toggle();
    _delay_us(SST_WE_HIGH_TIME_US);
}

void pcbhal_166_enter_loadmode()
{
    pcbhal_so_shld_clear();
}

void pcbhal_166_enter_shiftmode()
{
    pcbhal_so_shld_set();
}

void pcbhal_166_single_clock_run()
{
    _delay_us(SO_CLK_HALF_PERIOD_US);
    pcbhal_so_clk_toggle();
    _delay_us(SO_CLK_HALF_PERIOD_US);
    pcbhal_so_clk_toggle();
}

void pcbhal_166_clear()
{
    _delay_us(1.0f);
    pcbhal_so_clr_toggle();
    _delay_us(1.0f);
    pcbhal_so_clr_toggle();
}
