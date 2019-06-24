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
    pcbhal_sst_chip_enable();
    pcbhal_sst_outs_enable();
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
    pcbhal_sst_chip_disable();
    pcbhal_sst_outs_disable();
    pcbhal_4245_outs_disable();
    pcbhal_166_enter_loadmode();
    _delay_us(1.0f);

    // 7. Return read data
    *retbt = gained_val;

    return SST_SUCCESS;
}

/*
 * @brief Puts data (8-bit) and addr (24-bit) onto 595 chain.
 *
 * @param addr An addr under which the data shall be put. Only 24 bits are valid.
 * @param data Data to be put under given addr.
 */
void SSTADPut(
        uint32_t addr,
        uint8_t data)
{
    addr &= ~(0xFF000000); // zero-out addr msb 8 bits, only 24 bits are valid now
    uint32_t d_a = (((uint32_t)data)<<24) | addr; // data and addr concatenated into single var

    const uint8_t max_bits = 32;
    for(uint8_t i = 0; i < max_bits; i++)
    {
        uint8_t bit = ((uint8_t)(d_a>>((max_bits-1)-i))&0x01);
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

    // reload
    pcbhal_595_r_single_clock_run();
    _delay_us(1.0f);
}

sst_ec_t SSTWrite(
        uint32_t addr,
        uint8_t writebt)
{
    // prepare flash chip
    pcbhal_sst_chip_enable();
    pcbhal_sst_write_enable();

    // prepare shift regs
    pcbhal_595_clear();
    pcbhal_595a_outs_enable();
    pcbhal_595d_outs_enable();

    // prepare 4245
    pcbhal_4245_set_ab_outs_enable();
    _delay_us(1.0f);

    SSTADPut(SST_CMD_BYTE_PROG_CYCLE1_ADDR, SST_CMD_BYTE_PROG_CYCLE1_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(SST_CMD_BYTE_PROG_CYCLE2_ADDR, SST_CMD_BYTE_PROG_CYCLE2_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(SST_CMD_BYTE_PROG_CYCLE3_ADDR, SST_CMD_BYTE_PROG_CYCLE3_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(addr, writebt);
    pcbhal_sst_w_single_clock_run();

    _delay_us(SST_BYTE_PROGRAM_TIME_US);

    // get back to previous state
    pcbhal_sst_chip_disable();
    pcbhal_sst_write_disable();
    pcbhal_595a_outs_disable();
    pcbhal_595d_outs_disable();
    pcbhal_4245_outs_disable();

    return SST_SUCCESS;
}

sst_ec_t SSTEraseSector(uint32_t sector_addr)
{
    if((sector_addr > BOARD_MAX_SECTOR_ADDRESS) || (sector_addr < BOARD_MIN_SECTOR_ADDRESS))
    {
        return SST_INVALID_INPUT;
    }

    // get actual address, since bits A20-A12 are treated as sector address
    uint32_t s_addr = sector_addr << 12;

    // prepare flash chip
    pcbhal_sst_chip_enable();
    pcbhal_sst_write_enable();

    // prepare shift regs
    pcbhal_595_clear();
    pcbhal_595a_outs_enable();
    pcbhal_595d_outs_enable();

    // prepare 4245
    pcbhal_4245_set_ab_outs_enable();
    _delay_us(1.0f);

    SSTADPut(SST_CMD_SEC_ERASE_CYCLE1_ADDR, SST_CMD_SEC_ERASE_CYCLE1_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(SST_CMD_SEC_ERASE_CYCLE2_ADDR, SST_CMD_SEC_ERASE_CYCLE2_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(SST_CMD_SEC_ERASE_CYCLE3_ADDR, SST_CMD_SEC_ERASE_CYCLE3_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(SST_CMD_SEC_ERASE_CYCLE4_ADDR, SST_CMD_SEC_ERASE_CYCLE4_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(SST_CMD_SEC_ERASE_CYCLE5_ADDR, SST_CMD_SEC_ERASE_CYCLE5_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(s_addr, SST_CMD_SEC_ERASE_CYCLE6_DATA);
    pcbhal_sst_w_single_clock_run();

    _delay_ms(SST_SECTOR_ERASE_TIME_MS);

    // get back to previous state
    pcbhal_sst_chip_disable();
    pcbhal_sst_write_disable();
    pcbhal_595a_outs_disable();
    pcbhal_595d_outs_disable();
    pcbhal_4245_outs_disable();

    return SST_SUCCESS;
}

sst_ec_t SSTEraseBlock(uint32_t block_addr)
{
    if((block_addr > BOARD_MAX_BLOCK_ADDRESS) || (block_addr < BOARD_MIN_BLOCK_ADDRESS))
    {
        return SST_INVALID_INPUT;
    }

    // get actual address, since bits A20-A16 are treated as block address
    uint32_t b_addr = block_addr << 16;

    // prepare flash chip
    pcbhal_sst_chip_enable();
    pcbhal_sst_write_enable();

    // prepare shift regs
    pcbhal_595_clear();
    pcbhal_595a_outs_enable();
    pcbhal_595d_outs_enable();

    // prepare 4245
    pcbhal_4245_set_ab_outs_enable();
    _delay_us(1.0f);

    SSTADPut(SST_CMD_BLOCK_ERASE_CYCLE1_ADDR, SST_CMD_BLOCK_ERASE_CYCLE1_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(SST_CMD_BLOCK_ERASE_CYCLE2_ADDR, SST_CMD_BLOCK_ERASE_CYCLE2_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(SST_CMD_BLOCK_ERASE_CYCLE3_ADDR, SST_CMD_BLOCK_ERASE_CYCLE3_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(SST_CMD_BLOCK_ERASE_CYCLE4_ADDR, SST_CMD_BLOCK_ERASE_CYCLE4_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(SST_CMD_BLOCK_ERASE_CYCLE5_ADDR, SST_CMD_BLOCK_ERASE_CYCLE5_DATA);
    pcbhal_sst_w_single_clock_run();

    SSTADPut(b_addr, SST_CMD_BLOCK_ERASE_CYCLE6_DATA);
    pcbhal_sst_w_single_clock_run();

    _delay_ms(SST_BLOCK_ERASE_TIME_MS);

    // get back to previous state
    pcbhal_sst_chip_disable();
    pcbhal_sst_write_disable();
    pcbhal_595a_outs_disable();
    pcbhal_595d_outs_disable();
    pcbhal_4245_outs_disable();

    return SST_SUCCESS;
}
