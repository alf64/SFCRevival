/*
 * usr_msg.c
 *
 *  Created on: 8 maj 2019
 *      Author: bmudlaff
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Comm/comm.h"
#include "Ascii/ascii.h"
#include "usr_msg.h"

const char usr_msg_main_menu[USR_MSG_MAIN_AVAILABLE_USR_MSGS][USR_MSG_MAIN_MAX_CHARS_PER_MSG] PROGMEM = {
        {"\nWelcome to memi8M!"},
        {"Please choose an option:"},
        {"1. Read bytes (interactive)."},
        {"2. Write bytes (interactive)."},
        {"3. Read bytes."},
        {"4. Write bytes."},
        {"5. Read all memory."},
        {"6. Write all memory."},
        {"7. Erase sectors."},
        {"8. Erase blocks."},
        {"9. Erase all memory."},
        {"a. Check Product ID."},
        {"b. Debug scenario 1."},
        {"c. Debug scenario 2."},
        {"Select [1 - c] (ascii format): "}
};

const char usr_msg_unsupported_sel[23] PROGMEM = {
        "Unsupported selection."
};

const char usr_msg_critical_err[36] PROGMEM = {
        "Critical error, system halted here."
};

const char usr_msg_input_received[16] PROGMEM = {
        "Input received."
};

const char usr_msg_invalid_input_not_hex[26] PROGMEM = {
        "Invalid input: not a hex."
};

const char usr_msg_proceed_prompt[24] PROGMEM = {
        "Proceed ? ('y' / 'n'): "
};

const char usr_msg_retry_prompt[22] PROGMEM = {
        "Retry ? ('y' / 'n'): "
};

const char usr_msg_job_done[11] PROGMEM = {
        "Job done. "
};

const char usr_msg_prog_to_restart[23] PROGMEM = {
        "Restarting program... "
};

const char usr_msg_system_halted[16] PROGMEM = {
        "System halted. "
};

//!< ----- Prompts for read & write bytes -----
const char usr_msg_addr_prompt[26] PROGMEM = {
        "Provide starting address "
};
const char usr_msg_addr_fmt_advice[71] PROGMEM = {
        "(use 8-char hex ascii from range 00000000 - 001FFFFF, i.e. 0000ABCD): "
};
const char usr_msg_no_bytes_prompt[25] PROGMEM = {
        "Provide number of bytes "
};
const char usr_msg_no_bytes_fmt_advice[71] PROGMEM = {
        "(use 8-char hex ascii from range 00000001 - 00200000, i.e. 0000ABCD): "
};
const char usr_msg_given_addr_is[19] PROGMEM = {
        "Given address is: "
};
const char usr_msg_given_no_bytes_is[24] PROGMEM = {
        "Given number of elems: "
};
const char usr_msg_addr_out_of_range_err[35] PROGMEM = {
        "Error, given address out of range."
};
const char usr_msg_bts_out_of_range_err[33] PROGMEM = {
        "Error, given bytes out of range."
};
const char usr_msg_addr_vs_bts_err[40] PROGMEM = {
        "Error, given addr + bytes out of range."
};
const char usr_msg_data_format_prompt[44] PROGMEM = {
        "Choose data format ('1'=ASCII, '0'=bytes): "
};
const char usr_msg_data_format_invalid_err[34] PROGMEM = {
        "Error, given data format invalid."
};
const char usr_msg_given_data_format_is[23] PROGMEM = {
        "Given data format is: "
};
const char usr_msg_data_format_ascii[6] PROGMEM = {
        "ASCII"
};
const char usr_msg_data_format_bytes[6] PROGMEM = {
        "bytes"
};
const char usr_msg_data_ascii_prompt[43] PROGMEM = {
        "Provide data (use ascii, range: 00 - FF): "
};
const char usr_msg_data_raw_prompt[26] PROGMEM = {
        "Provide data (raw byte): "
};
const char usr_msg_progress_info_1[] PROGMEM = {
        "Progress: "
};
const char usr_msg_progress_info_2[] PROGMEM = {
        " out of "
};
const char usr_msg_progress_info_3[] PROGMEM = {
        " done. Left: "
};
const char usr_msg_read_data_info[15] PROGMEM = {
        "Read data is: "
};
const char usr_msg_write_data_atten[83] PROGMEM = {
        "Attention! For write to take effect, make sure you have erased flash memory first."
};
const char usr_msg_memcap_prompt[59] PROGMEM = {
        "Choose memory capacity to operate on ('1'=16Mb, '0'=8Mb):"
};
const char usr_msg_memcap_invalid_err[38] PROGMEM = {
        "Error, given memory capacity invalid."
};
// --------------------------------------------

//!< ----- Infos for read & write all -----
const char usr_msg_readall_info[215] PROGMEM = {
        "Attempting to read the memory block...\n"
        "Attention!\n"
        "Upon proceeding:\n"
        "*Read data will be provided as raw bytes.\n"
        "*There won't be any messages send besides the data.\n"
        "*Program will halt forever upon reading all the data."
};
const char usr_msg_writeall_info[344] PROGMEM = {
        "Attempting to write the memory block...\n"
        "Attention!\n"
        "Upon proceeding:\n"
        "*Program expects user to provide all the data as raw bytes.\n"
        "*About 2 ms of delay needs to be applied between each byte,\n"
        "because uC needs time to flash it on memory.\n"
        "*Confirmation message will be send upon writing all the data.\n"
        "*Program will continue upon writing all the data."
};
const char usr_msg_memsize_is[31] PROGMEM = {
        "Total number of data (bytes): "
};
const char usr_msg_all_data_prompt[31] PROGMEM = {
        "Provide the data (raw bytes): "
};
// --------------------------------------------

//!< -----------Prompts for erase ------------
const char usr_msg_sector_addr_prompt[24] PROGMEM = {
        "Provide sector address "
};
const char usr_msg_sector_addr_fmt_advice[71] PROGMEM = {
        "(use 8-char hex ascii from range 00000000 - 000001FF, i.e. 000000AB): "
};
const char usr_msg_block_addr_prompt[23] PROGMEM = {
        "Provide block address "
};
const char usr_msg_block_addr_fmt_advice[71] PROGMEM = {
        "(use 8-char hex ascii from range 00000000 - 0000001F, i.e. 0000000A): "
};
const char usr_msg_sector_addr_out_of_range_err[42] PROGMEM = {
        "Error, given sector address out of range."
};
const char usr_msg_block_addr_out_of_range_err[41] PROGMEM = {
        "Error, given block address out of range."
};
const char usr_msg_eraseall_info[35] PROGMEM = {
        "Attempting to erase chip memory..."
};
const char usr_msg_sectors_cnt_prompt[27] PROGMEM = {
        "Provide number of sectors "
};
const char usr_msg_sectors_cnt_fmt_advice[71] PROGMEM = {
        "(use 8-char hex ascii from range 00000001 - 00000200, i.e. 000000AB): "
};
const char usr_msg_sectors_cnt_out_of_range_err[45] PROGMEM = {
        "Error, given number of sectors out of range."
};
const char usr_msg_addr_vs_sectors_cnt_err[52] PROGMEM = {
        "Error, given addr + number of sectors out of range."
};
const char usr_msg_blocks_cnt_prompt[26] PROGMEM = {
        "Provide number of blocks "
};
const char usr_msg_blocks_cnt_fmt_advice[71] PROGMEM = {
        "(use 8-char hex ascii from range 00000001 - 00000020, i.e. 0000001A): "
};
const char usr_msg_blocks_cnt_out_of_range_err[44] PROGMEM = {
        "Error, given number of blocks out of range."
};
const char usr_msg_addr_vs_blocks_cnt_err[51] PROGMEM = {
        "Error, given addr + number of blocks out of range."
};
// --------------------------------------------

//!< --------- Check for product id -----------
const char usr_msg_check_prod_id_info[34] PROGMEM = {
        "Attempting to check Product Id..."
};
const char usr_msg_man_id_is[18] PROGMEM = {
        "Manufacturer Id: "
};
const char usr_msg_dev_id_is[12] PROGMEM = {
        "Device Id: "
};
const char usr_msg_man_id_recognized_as[32] PROGMEM = {
        "Manufacturer Id recognized as: "
};
const char usr_msg_dev_id_recognized_as[26] PROGMEM = {
        "Device Id recognized as: "
};
const char usr_msg_man_id_unrecognized[31] PROGMEM = {
        "Manufacturer Id unrecognized. "
};
const char usr_msg_dev_id_unrecognized[25] PROGMEM = {
        "Device Id unrecognized. "
};
const char usr_msg_man_id_name[MEM_MANUFACTURER_NAME_LEN] PROGMEM = {
        MEM_MANUFACTURER_NAME
};
const char usr_msg_dev_id_name[MEM_DEVICE_NAME_LEN] PROGMEM = {
        MEM_DEVICE_NAME
};
// --------------------------------------------

usr_msg_status_t UsrMsgAskForAddr(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint32_t* addr)
{
    if((usr_input_buff == NULL) || (usr_input_buff_size < 8) || (addr == NULL))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_addr_prompt,
            (sizeof(usr_msg_addr_prompt)-1),
            1);
    CommSendMsgFromFlash(
            usr_msg_addr_fmt_advice,
            (sizeof(usr_msg_addr_fmt_advice)-1),
            1);
    CommCleanMsgBuffer();
    while(CommGetMsg(8, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    ascii_status_t ascii_status = HexAsciiToU32(usr_input_buff, addr);
    if(ascii_status == ASCII_INVALID_RANGE)
    {
        CommSendMsgFromFlash(
                usr_msg_invalid_input_not_hex,
                (sizeof(usr_msg_invalid_input_not_hex)-1), 1);
        return USR_MSG_INVALID_INPUT;
    }
    else if(ascii_status == ASCII_FAILED)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1), 1);
        return USR_MSG_FAILED;
    }
    else
    {
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgAskForSectorAddr(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint32_t* sector_addr)
{
    if((usr_input_buff == NULL) || (usr_input_buff_size < 8) || (sector_addr == NULL))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_sector_addr_prompt,
            (sizeof(usr_msg_sector_addr_prompt)-1),
            1);
    CommSendMsgFromFlash(
            usr_msg_sector_addr_fmt_advice,
            (sizeof(usr_msg_sector_addr_fmt_advice)-1),
            1);
    CommCleanMsgBuffer();
    while(CommGetMsg(8, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    ascii_status_t ascii_status = HexAsciiToU32(usr_input_buff, sector_addr);
    if(ascii_status == ASCII_INVALID_RANGE)
    {
        CommSendMsgFromFlash(
                usr_msg_invalid_input_not_hex,
                (sizeof(usr_msg_invalid_input_not_hex)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if(ascii_status == ASCII_FAILED)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgAskForSectorCount(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint32_t* sector_cnt)
{
    if((usr_input_buff == NULL) || (usr_input_buff_size < 8) || (sector_cnt == NULL))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_sectors_cnt_prompt,
            (sizeof(usr_msg_sectors_cnt_prompt)-1),
            1);
    CommSendMsgFromFlash(
            usr_msg_sectors_cnt_fmt_advice,
            (sizeof(usr_msg_sectors_cnt_fmt_advice)-1),
            1);
    CommCleanMsgBuffer();
    while(CommGetMsg(8, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    ascii_status_t ascii_status = HexAsciiToU32(usr_input_buff, sector_cnt);
    if(ascii_status == ASCII_INVALID_RANGE)
    {
        CommSendMsgFromFlash(
                usr_msg_invalid_input_not_hex,
                (sizeof(usr_msg_invalid_input_not_hex)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if(ascii_status == ASCII_FAILED)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgAskForBlockAddr(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint32_t* block_addr)
{
    if((usr_input_buff == NULL) || (usr_input_buff_size < 8) || (block_addr == NULL))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_block_addr_prompt,
            (sizeof(usr_msg_block_addr_prompt)-1),
            1);
    CommSendMsgFromFlash(
            usr_msg_block_addr_fmt_advice,
            (sizeof(usr_msg_block_addr_fmt_advice)-1),
            1);
    CommCleanMsgBuffer();
    while(CommGetMsg(8, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    ascii_status_t ascii_status = HexAsciiToU32(usr_input_buff, block_addr);
    if(ascii_status == ASCII_INVALID_RANGE)
    {
        CommSendMsgFromFlash(
                usr_msg_invalid_input_not_hex,
                (sizeof(usr_msg_invalid_input_not_hex)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if(ascii_status == ASCII_FAILED)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgAskForBlockCount(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint32_t* block_cnt)
{
    if((usr_input_buff == NULL) || (usr_input_buff_size < 8) || (block_cnt == NULL))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_blocks_cnt_prompt,
            (sizeof(usr_msg_blocks_cnt_prompt)-1),
            1);
    CommSendMsgFromFlash(
            usr_msg_blocks_cnt_fmt_advice,
            (sizeof(usr_msg_blocks_cnt_fmt_advice)-1),
            1);
    CommCleanMsgBuffer();
    while(CommGetMsg(8, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    ascii_status_t ascii_status = HexAsciiToU32(usr_input_buff, block_cnt);
    if(ascii_status == ASCII_INVALID_RANGE)
    {
        CommSendMsgFromFlash(
                usr_msg_invalid_input_not_hex,
                (sizeof(usr_msg_invalid_input_not_hex)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if(ascii_status == ASCII_FAILED)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgAskForNoOfBts(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint32_t* bts)
{
    if((usr_input_buff == NULL) || (usr_input_buff_size < 8) || (bts == NULL))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_no_bytes_prompt,
            (sizeof(usr_msg_no_bytes_prompt)-1),
            1);
    CommSendMsgFromFlash(
            usr_msg_no_bytes_fmt_advice,
            (sizeof(usr_msg_no_bytes_fmt_advice)-1),
            1);
    CommCleanMsgBuffer();
    while(CommGetMsg(8, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    ascii_status_t ascii_status = HexAsciiToU32(usr_input_buff, bts);
    if(ascii_status == ASCII_INVALID_RANGE)
    {
        CommSendMsgFromFlash(
                usr_msg_invalid_input_not_hex,
                (sizeof(usr_msg_invalid_input_not_hex)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if(ascii_status == ASCII_FAILED)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgAskForDataFmt(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        usr_data_fmt* fmt)
{
    if((usr_input_buff == NULL) || (usr_input_buff_size < 1) || (fmt == NULL))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_data_format_prompt,
            (sizeof(usr_msg_data_format_prompt)-1),
            1);
    CommCleanMsgBuffer();
    while(CommGetMsg(1, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    if((usr_input_buff[0] != USR_DATA_FMT_BYTES) && (usr_input_buff[0] != USR_DATA_FMT_ASCII))
    {
        CommSendMsgFromFlash(
                usr_msg_data_format_invalid_err,
                (sizeof(usr_msg_data_format_invalid_err)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else
    {
        *fmt = usr_input_buff[0];
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgAskForMemoryCapacity(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        usr_memcap_t* memcap)
{
    if((usr_input_buff == NULL) || (usr_input_buff_size < 1) || (memcap == NULL))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_memcap_prompt,
            (sizeof(usr_msg_memcap_prompt)-1),
            1);
    CommCleanMsgBuffer();
    while(CommGetMsg(1, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    if((usr_input_buff[0] != USR_MEMCAP_8Mb) && (usr_input_buff[0] != USR_MEMCAP_16Mb))
    {
        CommSendMsgFromFlash(
                usr_msg_memcap_invalid_err,
                (sizeof(usr_msg_memcap_invalid_err)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else
    {
        *memcap = usr_input_buff[0];
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgAskForProceed(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint8_t* proceed)
{
    if((usr_input_buff == NULL) || (usr_input_buff_size < 1) || (proceed == NULL))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_proceed_prompt,
            (sizeof(usr_msg_proceed_prompt)-1),
            1);
    CommCleanMsgBuffer();
    while(CommGetMsg(1, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);

    if((usr_input_buff[0] != 'y') && (usr_input_buff[0] != 'n')
            && (usr_input_buff[0] != 'Y') && (usr_input_buff[0] != 'N'))
    {
        CommSendMsgFromFlash(
                usr_msg_unsupported_sel,
                (sizeof(usr_msg_unsupported_sel)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if((usr_input_buff[0] == 'n') || (usr_input_buff[0] == 'N'))
    {
        *proceed = 0;
        return USR_MSG_SUCCESS;
    }
    else // (usr_input[0] == 'y') || (usr_input[0] == 'Y')
    {
        *proceed = 1;
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgAskForRetry(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint8_t* retry)
{
    if((usr_input_buff == NULL) || (usr_input_buff_size < 1) || (retry == NULL))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_retry_prompt,
            (sizeof(usr_msg_retry_prompt)-1),
            1);
    CommCleanMsgBuffer();
    while(CommGetMsg(1, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    if((usr_input_buff[0] != 'y') && (usr_input_buff[0] != 'n')
            && (usr_input_buff[0] != 'Y') && (usr_input_buff[0] != 'N'))
    {
        CommSendMsgFromFlash(
                usr_msg_unsupported_sel,
                (sizeof(usr_msg_unsupported_sel)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if((usr_input_buff[0] == 'n') || (usr_input_buff[0] == 'N'))
    {
        *retry = 0;
        return USR_MSG_SUCCESS;
    }
    else // (usr_input[0] == 'y') || (usr_input[0] == 'Y')
    {
        *retry = 1;
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgAskForSingleData(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        usr_data_fmt fmt,
        uint8_t* data)
{
    if((usr_input_buff == NULL) || (usr_input_buff_size < 2) || (data == NULL))
        return USR_MSG_FAILED;

    uint8_t bts_needed = 0;
    if(fmt == USR_DATA_FMT_ASCII)
    {
        CommSendMsgFromFlash(
                usr_msg_data_ascii_prompt,
                (sizeof(usr_msg_data_ascii_prompt)-1),
                1);
        bts_needed = 2;
    }
    else // USR_DATA_FMT_BYTES
    {
        CommSendMsgFromFlash(
                usr_msg_data_raw_prompt,
                (sizeof(usr_msg_data_raw_prompt)-1),
                1);
        bts_needed = 1;
    }

    CommCleanMsgBuffer();
    while(CommGetMsg(bts_needed, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

    if(fmt == USR_DATA_FMT_ASCII)
    {
        ascii_status_t ascii_status = HexAsciiToU8(usr_input_buff, data);
        if(ascii_status == ASCII_INVALID_RANGE)
        {
            CommSendMsgFromFlash(
                    usr_msg_invalid_input_not_hex,
                    (sizeof(usr_msg_invalid_input_not_hex)-1),
                    1);
            return USR_MSG_INVALID_INPUT;
        }
        else if(ascii_status == ASCII_FAILED)
        {
            CommSendMsgFromFlash(
                    usr_msg_critical_err,
                    (sizeof(usr_msg_critical_err)-1),
                    1);
            return USR_MSG_FAILED;
        }
        else
        {
            // this must be success, do nothing
        }
    }
    else // USR_DATA_FMT_BYTES
    {
        *data = usr_input_buff[0];
    }

    return USR_MSG_SUCCESS;
}

usr_msg_status_t UsrMsgDispAddrAsAscii(
        uint32_t addr,
        unsigned char* workbuff,
        uint8_t workbuff_size)
{
    if((workbuff == NULL) || (workbuff_size < 9))
        return USR_MSG_FAILED;

    ascii_status_t ascii_status = U32ToHexAscii(
            addr,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_given_addr_is,
                (sizeof(usr_msg_given_addr_is)-1),
                0);
        CommSendMsg(
                workbuff,
                8,
                1);
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgDispNoOfBtsAsAscii(
        uint32_t bts,
        unsigned char* workbuff,
        uint8_t workbuff_size)
{
    if((workbuff == NULL) || (workbuff_size < 9))
        return USR_MSG_FAILED;

    ascii_status_t ascii_status = U32ToHexAscii(
            bts,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_given_no_bytes_is,
                (sizeof(usr_msg_given_no_bytes_is)-1),
                0);
        CommSendMsg(
                workbuff,
                8,
                1);
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgDispDataFmtAsAscii(usr_data_fmt fmt)
{
    CommSendMsgFromFlash(
            usr_msg_given_data_format_is,
            (sizeof(usr_msg_given_data_format_is)-1),
            0);
    if(fmt == USR_DATA_FMT_ASCII)
    {
        CommSendMsgFromFlash(
                usr_msg_data_format_ascii,
                (sizeof(usr_msg_data_format_ascii)-1),
                1);
        return USR_MSG_SUCCESS;
    }
    else if(fmt == USR_DATA_FMT_BYTES)
    {
        CommSendMsgFromFlash(
                usr_msg_data_format_bytes,
                (sizeof(usr_msg_data_format_bytes)-1),
                1);
        return USR_MSG_SUCCESS;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
}

usr_msg_status_t UsrMsgDispProdIdAsAscii(
        uint8_t man_id,
        uint8_t dev_id,
        unsigned char* workbuff,
        uint8_t workbuff_size)
{
    if((workbuff == NULL) || (workbuff_size < 3))
        return USR_MSG_FAILED;

    ascii_status_t ascii_status = U8ToHexAscii(
            man_id,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_man_id_is,
                (sizeof(usr_msg_man_id_is)-1),
                0);
        CommSendMsg(
                workbuff,
                2,
                1);
    }

    ascii_status = U8ToHexAscii(
            dev_id,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_dev_id_is,
                (sizeof(usr_msg_dev_id_is)-1),
                0);
        CommSendMsg(
                workbuff,
                2,
                1);
    }

    return USR_MSG_SUCCESS;
}

usr_msg_status_t UsrMsgDispProgress(
        uint32_t current,
        uint32_t limit,
        unsigned char* workbuff,
        uint8_t workbuff_size)
{
    if((workbuff == NULL) || (workbuff_size < 9))
        return USR_MSG_FAILED;

    if(current > limit)
        return USR_MSG_INVALID_INPUT;

    ascii_status_t ascii_status = U32ToHexAscii(
            current,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_progress_info_1,
                (sizeof(usr_msg_progress_info_1)-1),
                0);
        CommSendMsg(
                workbuff,
                8,
                0);
    }

    ascii_status = U32ToHexAscii(
            limit,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_progress_info_2,
                (sizeof(usr_msg_progress_info_2)-1),
                0);
        CommSendMsg(
                workbuff,
                8,
                0);
    }

    uint32_t left = limit - current;
    ascii_status = U32ToHexAscii(
            left,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_progress_info_3,
                (sizeof(usr_msg_progress_info_3)-1),
                0);
        CommSendMsg(
                workbuff,
                8,
                1);
    }

    return USR_MSG_SUCCESS;
}

usr_msg_status_t UsrMsgAddrBtsCheck(
        uint32_t addr,
        uint32_t bts)
{
    if((addr > BOARD_MAX_ADDRESS) || (addr < BOARD_MIN_ADDRESS))
    {
        CommSendMsgFromFlash(
                usr_msg_addr_out_of_range_err,
                (sizeof(usr_msg_addr_out_of_range_err)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if((bts > BOARD_SPACE_CAPACITY) || (bts == 0))
    {
        CommSendMsgFromFlash(
                usr_msg_bts_out_of_range_err,
                (sizeof(usr_msg_bts_out_of_range_err)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if(((addr + bts)-1) > BOARD_MAX_ADDRESS)
    {
        CommSendMsgFromFlash(
                usr_msg_addr_vs_bts_err,
                (sizeof(usr_msg_addr_vs_bts_err)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else
    {
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgSectorAddrCntCheck(
        uint32_t sector_addr,
        uint32_t sector_cnt)
{
    if((sector_addr > BOARD_MAX_SECTOR_ADDRESS) || (sector_addr < BOARD_MIN_SECTOR_ADDRESS))
    {
        CommSendMsgFromFlash(
                usr_msg_sector_addr_out_of_range_err,
                (sizeof(usr_msg_sector_addr_out_of_range_err)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if((sector_cnt > BOARD_SECTOR_COUNT) || (sector_cnt == 0))
    {
        CommSendMsgFromFlash(
                usr_msg_sectors_cnt_out_of_range_err,
                (sizeof(usr_msg_sectors_cnt_out_of_range_err)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if( ((sector_addr + sector_cnt) >  BOARD_SECTOR_COUNT) )
    {
        CommSendMsgFromFlash(
                usr_msg_addr_vs_sectors_cnt_err,
                (sizeof(usr_msg_addr_vs_sectors_cnt_err)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else
    {
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgBlockAddrCntCheck(
        uint32_t block_addr,
        uint32_t block_cnt)
{
    if((block_addr > BOARD_MAX_BLOCK_ADDRESS) || (block_addr < BOARD_MIN_BLOCK_ADDRESS))
    {
        CommSendMsgFromFlash(
                usr_msg_block_addr_out_of_range_err,
                (sizeof(usr_msg_block_addr_out_of_range_err)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if((block_cnt > BOARD_BLOCK_COUNT) || (block_cnt == 0))
    {
        CommSendMsgFromFlash(
                usr_msg_blocks_cnt_out_of_range_err,
                (sizeof(usr_msg_blocks_cnt_out_of_range_err)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else if( ((block_addr + block_cnt) >  BOARD_BLOCK_COUNT) )
    {
        CommSendMsgFromFlash(
                usr_msg_addr_vs_blocks_cnt_err,
                (sizeof(usr_msg_addr_vs_blocks_cnt_err)-1),
                1);
        return USR_MSG_INVALID_INPUT;
    }
    else
    {
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgProdIdCheck(
        uint8_t man_id,
        uint8_t dev_id)
{
    usr_msg_status_t result = USR_MSG_SUCCESS;

    if(man_id == MEM_MANUFACTURER_ID)
    {
        CommSendMsgFromFlash(
                usr_msg_man_id_recognized_as,
                (sizeof(usr_msg_man_id_recognized_as)-1),
                0);
        CommSendMsgFromFlash(
                usr_msg_man_id_name,
                (sizeof(usr_msg_man_id_name)-1),
                1);
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_man_id_unrecognized,
                (sizeof(usr_msg_man_id_unrecognized)-1),
                1);
        result = USR_MSG_INVALID_INPUT;
    }

    if(dev_id == MEM_DEVICE_ID)
    {
        CommSendMsgFromFlash(
                usr_msg_dev_id_recognized_as,
                (sizeof(usr_msg_dev_id_recognized_as)-1),
                0);
        CommSendMsgFromFlash(
                usr_msg_dev_id_name,
                (sizeof(usr_msg_dev_id_name)-1),
                1);
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_dev_id_unrecognized,
                (sizeof(usr_msg_dev_id_unrecognized)-1),
                1);
        result = USR_MSG_INVALID_INPUT;
    }

    return result;
}

usr_msg_status_t UsrMsgDispReadAllInfo(
        uint32_t mem_size,
        unsigned char* workbuff,
        uint8_t workbuff_size)
{
    if((workbuff == NULL) || (workbuff_size < 9))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_readall_info,
            (sizeof(usr_msg_readall_info)-1),
            1);

    ascii_status_t ascii_status = U32ToHexAscii(
            mem_size,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_memsize_is,
                (sizeof(usr_msg_memsize_is)-1),
                0);
        CommSendMsg(
                workbuff,
                8,
                1);
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgDispWriteAllInfo(
        uint32_t mem_size,
        unsigned char* workbuff,
        uint8_t workbuff_size)
{
    if((workbuff == NULL) || (workbuff_size < 9))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_writeall_info,
            (sizeof(usr_msg_writeall_info)-1),
            1);

    ascii_status_t ascii_status = U32ToHexAscii(
            mem_size,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                (sizeof(usr_msg_critical_err)-1),
                1);
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_memsize_is,
                (sizeof(usr_msg_memsize_is)-1),
                0);
        CommSendMsg(
                workbuff,
                8,
                1);
        return USR_MSG_SUCCESS;
    }
}
