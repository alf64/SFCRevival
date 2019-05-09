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
        {"5. Debug scenario 1."},
        {"6. Debug scenario 2."},
        {"Select [1 - 6] (ascii format): "}
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
const char usr_msg_output_format_prompt[] PROGMEM = {
        "Choose output format ('1'=ASCII, '0'=bytes): "
};
const char usr_msg_output_format_invalid_err[] PROGMEM = {
        "Error, given output format invalid."
};
const char usr_msg_given_output_format_is[] PROGMEM = {
        "Given output format is: "
};
const char usr_msg_given_output_format_ascii[] PROGMEM = {
        "ASCII"
};
const char usr_msg_given_output_format_bytes[] PROGMEM = {
        "bytes"
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

usr_msg_status_t UsrMsgAskForOutFmt(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        usr_out_fmt* fmt)
{
    if((usr_input_buff == NULL) || (usr_input_buff_size < 1) || (fmt == NULL))
        return USR_MSG_FAILED;

    CommSendMsgFromFlash(
            usr_msg_output_format_prompt,
            sizeof(usr_msg_output_format_prompt-1));
    CommCleanMsgBuffer();
    while(CommGetMsg(1, usr_input_buff, usr_input_buff_size) != COMM_SUCCESS);
    CommSendMsgFromFlash(usr_msg_input_received, sizeof(usr_msg_input_received-1));

    if((usr_input_buff[0] != USR_OUT_FMT_BYTES) && (usr_input_buff[0] != USR_OUT_FMT_ASCII))
    {
        CommSendMsgFromFlash(
                usr_msg_output_format_invalid_err,
                sizeof(usr_msg_output_format_invalid_err-1));
        return USR_MSG_INVALID_INPUT;
    }
    else
    {
        *fmt = usr_input_buff[0];
        return USR_MSG_SUCCESS;
    }
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

usr_msg_status_t UsrMsgDispOutFmtAsAscii(usr_out_fmt fmt)
{
    CommSendMsgFromFlash(
            usr_msg_given_output_format_is,
            sizeof(usr_msg_given_output_format_is-1));
    if(fmt == USR_OUT_FMT_ASCII)
    {
        CommSendMsgFromFlash(
                usr_msg_given_output_format_ascii,
                sizeof(usr_msg_given_output_format_ascii-1));
        return USR_MSG_SUCCESS;
    }
    else if(fmt == USR_OUT_FMT_BYTES)
    {
        CommSendMsgFromFlash(
                usr_msg_given_output_format_bytes,
                sizeof(usr_msg_given_output_format_bytes-1));
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

