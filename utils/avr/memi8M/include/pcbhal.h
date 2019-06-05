/*
 * pcbhal.h
 *
 *  Created on: 27 maj 2019
 *      Author: bmudlaff
 */

#ifndef INCLUDE_PCBHAL_H_
#define INCLUDE_PCBHAL_H_

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "boards/memi8M_pcb.h"

/*
 * HAL functions here for pcb controls.
 */

/*
 * @brief Inits functionality of all of the hal functions present here.
 *
 * @details Sets directions of the I/O pins.
 */
void pcbhal_init();

/*
 * @brief Sets SI_SER to '1'.
 */
static inline void pcbhal_si_ser_set()
{
    UC_PORT1 |= (1<<SI_SER);
}

/*
 * @brief Clears SI_SER to '0'.
 */
static inline void pcbhal_si_ser_clear()
{
    UC_PORT1 &= ~(1<<SI_SER);
}

/*
 * @brief Sets SI_SRCLK to '1'.
 */
static inline void pcbhal_si_srclk_set()
{
    UC_PORT1 |= (1<<SI_SRCLK);
}

/*
 * @brief Clears SI_SRCLK to '0'.
 */
static inline void pcbhal_si_srclk_clear()
{
    UC_PORT1 &= ~(1<<SI_SRCLK);
}

static inline void pcbhal_si_srclk_toggle()
{
    UC_PORT1 ^= (1<<SI_SRCLK);
}

/*
 * @brief Sets SI_SRCLR to '1'.
 */
static inline void pcbhal_si_srclr_set()
{
    UC_PORT1 |= (1<<SI_SRCLR);
}

/*
 * @brief Clears SI_SRCLR to '0'.
 */
static inline void pcbhal_si_srclr_clear()
{
    UC_PORT1 &= ~(1<<SI_SRCLR);
}

/*
 * @brief Toggles the SI_SRCLR.
 */
static inline void pcbhal_si_srclr_toggle()
{
    UC_PORT1 ^= (1<SI_SRCLR);
}

/*
 * @brief Sets SI_RCLK to '1'.
 */
static inline void pcbhal_si_rclk_set()
{
    UC_PORT1 |= (1<<SI_RCLK);
}

/*
 * @brief Clears SI_RCLK to '0'.
 */
static inline void pcbhal_si_rclk_clear()
{
    UC_PORT1 &= ~(1<<SI_RCLK);
}

/*
 * @brief Toggles the SI_RCLK.
 */
static inline void pcbhal_si_rclk_toggle()
{
    UC_PORT1 ^= (1<<SI_RCLK);
}

/*
 * @brief Sets SI_OE_A to '1'.
 */
static inline void pcbhal_si_oe_a_set()
{
    UC_PORT1 |= (1<<SI_OE_A);
}

/*
 * @brief Clears SI_OE_A to '0.
 */
static inline void pcbhal_si_oe_a_clear()
{
    UC_PORT1 &= ~(1<<SI_OE_A);
}

/*
 * @brief Sets SI_OE_D to '1'.
 */
static inline void pcbhal_si_oe_d_set()
{
    UC_PORT1 |= (1<<SI_OE_D);
}

/*
 * @brief Clears SI_OE_D to '0'.
 */
static inline void pcbhal_si_oe_d_clear()
{
    UC_PORT1 &= ~(1<<SI_OE_D);
}

/*
 * @brief Sets ROM_CE to '1'.
 */
static inline void pcbhal_rom_ce_set()
{
    UC_PORT1 |= (1<<ROM_CE);
}

/*
 * @brief Clears ROM_CE to '0'.
 */
static inline void pcbhal_rom_ce_clear()
{
    UC_PORT1 &= ~(1<<ROM_CE);
}

/*
 * @brief Sets ROM_OE to '1'.
 */
static inline void pcbhal_rom_oe_set()
{
    UC_PORT1 |= (1<<ROM_OE);
}

/*
 * @brief Clears ROM_OE to '0'.
 */
static inline void pcbhal_rom_oe_clear()
{
    UC_PORT1 &= ~(1<<ROM_OE);
}

/*
 * @brief Sets SO_SHLD to '1'.
 */
static inline void pcbhal_so_shld_set()
{
    UC_PORT2 |= (1<<SO_SHLD);
}

/*
 * @brief Clears SO_SHLD to '0'.
 */
static inline void pcbhal_so_shld_clear()
{
    UC_PORT2 &= ~(1<<SO_SHLD);
}

/*
 * @brief Sets SO_CLK to '1'.
 */
static inline void pcbhal_so_clk_set()
{
    UC_PORT2 |= (1<<SO_CLK);
}

/*
 * @brief Clears SO_CLK to '0'.
 */
static inline void pcbhal_so_clk_clear()
{
    UC_PORT2 &= ~(1<<SO_CLK);
}

/*
 * @brief Toggles the SO_CLK.
 */
static inline void pcbhal_so_clk_toggle()
{
    UC_PORT2 ^= (1<<SO_CLK);
}

/*
 * @brief Sets SO_CLR to '1'.
 */
static inline void pcbhal_so_clr_set()
{
    UC_PORT2 |= (1<<SO_CLR);
}

/*
 * @brief Clears SO_CLR to '0'.
 */
static inline void pcbhal_so_clr_clear()
{
    UC_PORT2 &= ~(1<<SO_CLR);
}

/*
 * @brief Toggles the SO_CLR.
 */
static inline void pcbhal_so_clr_toggle()
{
    UC_PORT2 ^= (1<<SO_CLR);
}

/*
 * @brief Sets DATA_DIR to '1'.
 */
static inline void pcbhal_data_dir_set()
{
    UC_PORT2 |= (1<<DATA_DIR);
}

/*
 *@brief Clears DATA_DIR to '0'.
 */
static inline void pcbhal_data_dir_clear()
{
    UC_PORT2 &= ~(1<<DATA_DIR);
}

/*
 * @brief Sets DATA_OE to '1'.
 */
static inline void pcbhal_data_oe_set()
{
    UC_PORT2 |= (1<<DATA_OE);
}

/*
 * @brief Clears DATA_OE to '0'.
 */
static inline void pcbhal_data_oe_clear()
{
    UC_PORT2 &= ~(1<<DATA_OE);
}

/*
 * @brief Sets WE_5V to '1'.
 */
static inline void pcbhal_we_5v_set()
{
    UC_PORT2 |= (1<<WE_5V);
}

/*
 * @brief Clears WE_5V to '0'.
 */
static inline void pcbhal_we_5v_clear()
{
    UC_PORT2 &= ~(1<<WE_5V);
}

/*
 * @brief Sets SO_QH to '1'.
 */
static inline void pcbhal_so_qh_set()
{
    UC_PORT2 |= (1<<SO_QH);
}

/*
 * @brief Clears SO_QH to '0'.
 */
static inline void pcbhal_so_qh_clear()
{
    UC_PORT2 &= ~(1<<SO_QH);
}

/*
 * @brief Returns the logical value of SO_QH ('0' or '1').
 */
static inline uint8_t pcbhal_so_qh_get()
{
    return ((UC_PIN2>>SO_QH)&0x01);
}

/*
 * @brief Sets data direction of 4245 to A -> B and enables its outputs.
 */
void pcbhal_4245_set_ab_outs_enable();

/*
 * @brief Sets data direction of 4245 to B -> A and enables its outputs.
 */
void pcbhal_4245_set_ba_outs_enable();

/*
 * @brief Disables outputs of 4245.
 */
void pcbhal_4245_outs_disable();

/*
 * @brief Enables outputs of 595 address regs.
 */
void pcbhal_595a_outs_enable();

/*
 * @brief Disables outputs of 595 address regs.
 */
void pcbhal_595a_outs_disable();

/*
 * @brief Enables outputs of 595 data reg.
 */
void pcbhal_595d_outs_enable();

/*
 * @brief Disables outputs of 595 data reg.
 */
void pcbhal_595d_outs_disable();

/*
 * @brief Clears (the internal regs of) all 595's.
 */
void pcbhal_595_clear();

/*
 * @brief Does single sr (shift regs) clock run on all 595's.
 */
void pcbhal_595_sr_single_clock_run();

/*
 * @brief Does single r (storage register) clock run on all 595's.
 */
void pcbhal_595_r_single_clock_run();

/*
 * @brief Enables SST mem chip.
 */
void pcbhal_sst_chip_enable();

/*
 * @brief Disables SST mem chip.
 */
void pcbhal_sst_chip_disable();

/*
 * @brief Enables SST chip outputs.
 */
void pcbhal_sst_outs_enable();

/*
 * @brief Disables SST chip outputs.
 */
void pcbhal_sst_outs_disable();

/*
 * @brief Turns 166 into load-mode (obtaining data from parallel inputs).
 */
void pcbhal_166_enter_loadmode();

/*
 * @brief Turns 166 into serial shift-mode
 * (serial input active, shifting obtained data serially onto serial output)..
 */
void pcbhal_166_enter_shiftmode();

/*
 * @brief Runs single clock on 166.
 */
void pcbhal_166_single_clock_run();

/*
 * @brief Clears internal register of 166.
 */
void pcbhal_166_clear();

#endif /* INCLUDE_PCBHAL_H_ */
