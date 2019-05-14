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
#include "boards/memi8M_pcb.h"

const char usr_msg_main_menu[USR_MSG_MAIN_AVAILABLE_USR_MSGS][USR_MSG_MAIN_MAX_CHARS_PER_MSG] PROGMEM = {
        {"\nWelcome to memi8M!"},
        {"Please choose an option:"},
        {"1. Read bytes."},
        {"2. Write bytes."},
        {"3. Read all memory."},
        {"4. Write all memory."},
        {"5. Erase sector."},
        {"6. Erase all memory."},
        {"7. Debug scenario 1."},
        {"8. Debug scenario 2."},
        {"Select [1 - 8] (ascii format): "}
};

const char usr_msg_unsupported_sel[] PROGMEM = {
        "Unsupported selection."
};

const char usr_msg_critical_err[] PROGMEM = {
        "Critical error, system halted here."
};

const char usr_msg_input_received[] PROGMEM = {
        "Input received."
};

const char usr_msg_invalid_input_not_hex[] PROGMEM = {
        "Invalid input: not a hex."
};

const char usr_msg_proceed_prompt[] PROGMEM = {
        "Proceed ? ('y' / 'n'): "
};

const char usr_msg_retry_prompt[] PROGMEM = {
        "Retry ? ('y' / 'n'): "
};

const char usr_msg_job_done[] PROGMEM = {
        "Job done. "
};

const char usr_msg_prog_to_restart[] PROGMEM = {
        "Restarting program... "
};

const char usr_msg_system_halted[] PROGMEM = {
        "System halted. "
};

//!< ----- Prompts for read & write bytes -----
const char usr_msg_addr_prompt[] PROGMEM = {
        "Provide starting address "
};
const char usr_msg_addr_fmt_advice[] PROGMEM = {
        "(use 8-char hex ascii from range 00000000 - 000FFFFF, i.e. 0000ABCD): "
};
const char usr_msg_no_bytes_prompt[] PROGMEM = {
        "Provide number of bytes "
};
const char usr_msg_no_bytes_fmt_advice[] PROGMEM = {
        "(use 8-char hex ascii from range 00000001 - 00100000, i.e. 0000ABCD): "
};
const char usr_msg_given_addr_is[] PROGMEM = {
        "Given address is: "
};
const char usr_msg_given_no_bytes_is[] PROGMEM = {
        "Given number of bytes: "
};
const char usr_msg_addr_out_of_range_err[] PROGMEM = {
        "Error, given address out of range."
};
const char usr_msg_bts_out_of_range_err[] PROGMEM = {
        "Error, given bytes out of range."
};
const char usr_msg_addr_vs_bts_err[] PROGMEM = {
        "Error, given addr + bytes out of range."
};
const char usr_msg_data_format_prompt[] PROGMEM = {
        "Choose data format ('1'=ASCII, '0'=bytes): "
};
const char usr_msg_data_format_invalid_err[] PROGMEM = {
        "Error, given data format invalid."
};
const char usr_msg_given_data_format_is[] PROGMEM = {
        "Given data format is: "
};
const char usr_msg_data_format_ascii[] PROGMEM = {
        "ASCII"
};
const char usr_msg_data_format_bytes[] PROGMEM = {
        "bytes"
};
const char usr_msg_data_ascii_prompt[] PROGMEM = {
        "Provide data (use ascii, range: 00 - FF): "
};
const char usr_msg_data_raw_prompt[] PROGMEM = {
        "Provide data (raw byte): "
};
// --------------------------------------------

//!< ----- Infos for read & write all -----
const char usr_msg_readall_info[] PROGMEM = {
        "Attempting to read the whole memory...\n"
        "Attention!\n"
        "Upon proceeding:\n"
        "*Read data will be provided as raw bytes.\n"
        "*There won't be any messages send besides the data.\n"
        "*Program will halt forever upon reading all the data.\n"
};
const char usr_msg_writeall_info[] PROGMEM = {
        "Attempting to write the whole memory...\n"
        "Attention!\n"
        "Upon proceeding:\n"
        "*Program expects user to provide all the data as raw bytes.\n"
        "*Confirmation message will be send upon writing all the data.\n"
        "*Program will continue upon writing all the data.\n"
};
const char usr_msg_memsize_is[] PROGMEM = {
        "Total number of data (bytes): "
};
const char usr_msg_all_data_prompt[] PROGMEM = {
        "Provide the data (raw bytes): "
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
            sizeof(usr_msg_addr_prompt-1));
    CommSendMsgFromFlash(
            usr_msg_addr_fmt_advice,
            sizeof(usr_msg_addr_fmt_advice-1));
    CommCleanMsgBuffer();
    while(CommGetMsg(8, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, sizeof(usr_msg_input_received-1));

    ascii_status_t ascii_status = AsciiToU32(usr_input_buff, addr);
    if(ascii_status == ASCII_INVALID_RANGE)
    {
        CommSendMsgFromFlash(
                usr_msg_invalid_input_not_hex,
                sizeof(usr_msg_invalid_input_not_hex-1));
        return USR_MSG_INVALID_INPUT;
    }
    else if(ascii_status == ASCII_FAILED)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                sizeof(usr_msg_critical_err-1));
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
            sizeof(usr_msg_no_bytes_prompt-1));
    CommSendMsgFromFlash(
            usr_msg_no_bytes_fmt_advice,
            sizeof(usr_msg_no_bytes_fmt_advice-1));
    CommCleanMsgBuffer();
    while(CommGetMsg(8, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, sizeof(usr_msg_input_received-1));

    ascii_status_t ascii_status = AsciiToU32(usr_input_buff, bts);
    if(ascii_status == ASCII_INVALID_RANGE)
    {
        CommSendMsgFromFlash(
                usr_msg_invalid_input_not_hex,
                sizeof(usr_msg_invalid_input_not_hex-1));
        return USR_MSG_INVALID_INPUT;
    }
    else if(ascii_status == ASCII_FAILED)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                sizeof(usr_msg_critical_err-1));
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
            sizeof(usr_msg_data_format_prompt-1));
    CommCleanMsgBuffer();
    while(CommGetMsg(1, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, sizeof(usr_msg_input_received-1));

    if((usr_input_buff[0] != USR_DATA_FMT_BYTES) && (usr_input_buff[0] != USR_DATA_FMT_ASCII))
    {
        CommSendMsgFromFlash(
                usr_msg_data_format_invalid_err,
                sizeof(usr_msg_data_format_invalid_err-1));
        return USR_MSG_INVALID_INPUT;
    }
    else
    {
        *fmt = usr_input_buff[0];
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
            sizeof(usr_msg_proceed_prompt-1));
    CommCleanMsgBuffer();
    while(CommGetMsg(1, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, sizeof(usr_msg_input_received-1));

    if((usr_input_buff[0] != 'y') && (usr_input_buff[0] != 'n')
            && (usr_input_buff[0] != 'Y') && (usr_input_buff[0] != 'N'))
    {
        CommSendMsgFromFlash(
                usr_msg_unsupported_sel,
                sizeof(usr_msg_unsupported_sel-1));
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
            sizeof(usr_msg_retry_prompt-1));
    CommCleanMsgBuffer();
    while(CommGetMsg(1, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, sizeof(usr_msg_input_received-1));

    if((usr_input_buff[0] != 'y') && (usr_input_buff[0] != 'n')
            && (usr_input_buff[0] != 'Y') && (usr_input_buff[0] != 'N'))
    {
        CommSendMsgFromFlash(
                usr_msg_unsupported_sel,
                sizeof(usr_msg_unsupported_sel-1));
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
                sizeof(usr_msg_data_ascii_prompt-1));
        bts_needed = 2;
    }
    else // USR_DATA_FMT_BYTES
    {
        CommSendMsgFromFlash(
                usr_msg_data_raw_prompt,
                sizeof(usr_msg_data_raw_prompt-1));
        bts_needed = 1;
    }

    CommCleanMsgBuffer();
    while(CommGetMsg(bts_needed, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, sizeof(usr_msg_input_received-1));

    if(fmt == USR_DATA_FMT_ASCII)
    {
        ascii_status_t ascii_status = AsciiToU8(usr_input_buff, data);
        if(ascii_status == ASCII_INVALID_RANGE)
        {
            CommSendMsgFromFlash(
                    usr_msg_invalid_input_not_hex,
                    sizeof(usr_msg_invalid_input_not_hex-1));
            return USR_MSG_INVALID_INPUT;
        }
        else if(ascii_status == ASCII_FAILED)
        {
            CommSendMsgFromFlash(
                    usr_msg_critical_err,
                    sizeof(usr_msg_critical_err-1));
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

    ascii_status_t ascii_status = U32ToAscii(
            addr,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                sizeof(usr_msg_critical_err-1));
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_given_addr_is,
                sizeof(usr_msg_given_addr_is-1));
        CommSendMsg(
                workbuff,
                8);
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

    ascii_status_t ascii_status = U32ToAscii(
            bts,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                sizeof(usr_msg_critical_err-1));
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_given_no_bytes_is,
                sizeof(usr_msg_given_no_bytes_is-1));
        CommSendMsg(
                workbuff,
                8);
        return USR_MSG_SUCCESS;
    }
}

usr_msg_status_t UsrMsgDispDataFmtAsAscii(usr_data_fmt fmt)
{
    CommSendMsgFromFlash(
            usr_msg_given_data_format_is,
            sizeof(usr_msg_given_data_format_is-1));
    if(fmt == USR_DATA_FMT_ASCII)
    {
        CommSendMsgFromFlash(
                usr_msg_data_format_ascii,
                sizeof(usr_msg_data_format_ascii-1));
        return USR_MSG_SUCCESS;
    }
    else if(fmt == USR_DATA_FMT_BYTES)
    {
        CommSendMsgFromFlash(
                usr_msg_data_format_bytes,
                sizeof(usr_msg_data_format_bytes-1));
        return USR_MSG_SUCCESS;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                sizeof(usr_msg_critical_err-1));
        return USR_MSG_FAILED;
    }
}

usr_msg_status_t UsrMsgAddrBtsCheck(
        uint32_t addr,
        uint32_t bts)
{
    if(addr > BOARD_MAX_ADDRESS)
    {
        CommSendMsgFromFlash(
                usr_msg_addr_out_of_range_err,
                sizeof(usr_msg_addr_out_of_range_err-1));
        return USR_MSG_INVALID_INPUT;
    }
    else if((bts > BOARD_SPACE_CAPACITY) || (bts == 0))
    {
        CommSendMsgFromFlash(
                usr_msg_bts_out_of_range_err,
                sizeof(usr_msg_bts_out_of_range_err-1));
        return USR_MSG_INVALID_INPUT;
    }
    else if(((addr + bts)-1) > BOARD_MAX_ADDRESS)
    {
        CommSendMsgFromFlash(
                usr_msg_addr_vs_bts_err,
                sizeof(usr_msg_addr_vs_bts_err-1));
        return USR_MSG_INVALID_INPUT;
    }
    else
    {
        return USR_MSG_SUCCESS;
    }
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
            sizeof(usr_msg_readall_info-1));

    ascii_status_t ascii_status = U32ToAscii(
            mem_size,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                sizeof(usr_msg_critical_err-1));
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_memsize_is,
                sizeof(usr_msg_memsize_is-1));
        CommSendMsg(
                workbuff,
                8);
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
            sizeof(usr_msg_writeall_info-1));

    ascii_status_t ascii_status = U32ToAscii(
            mem_size,
            workbuff,
            workbuff_size);
    if(ascii_status != ASCII_SUCCESS)
    {
        CommSendMsgFromFlash(
                usr_msg_critical_err,
                sizeof(usr_msg_critical_err-1));
        return USR_MSG_FAILED;
    }
    else
    {
        CommSendMsgFromFlash(
                usr_msg_memsize_is,
                sizeof(usr_msg_memsize_is-1));
        CommSendMsg(
                workbuff,
                8);
        return USR_MSG_SUCCESS;
    }
}
