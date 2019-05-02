/*
 * msg.h
 *
 *  Created on: 29 kwi 2019
 *      Author: bmudlaff
 */

#ifndef INCLUDE_USR_MSG_H_
#define INCLUDE_USR_MSG_H_

/*
 * This file contains messages for interaction with user.
 *
 */
#include <avr/pgmspace.h>

//!< messages
#define USR_MSG_MAIN_WELCOME 0
#define USR_MSG_MAIN_CHOOSE_OPTION_PROMPT 1
#define USR_MSG_MAIN_READ_BYTES_OPTION 2
#define USR_MSG_MAIN_WRITE_BYTES_OPTION 3
#define USR_MSG_MAIN_READ_ALL_OPTION 4
#define USR_MSG_MAIN_WRITE_ALL_OPTION 5
#define USR_MSG_MAIN_DEBUG_SCENARIO_01 6
#define USR_MSG_MAIN_DEBUG_SCENARIO_02 7
#define USR_MSG_MAIN_MAKE_SELECTION_PROMPT 8

//!< maximums
#define USR_MSG_MAIN_AVAILABLE_USR_MSGS 9
#define USR_MSG_MAIN_MAX_CHARS_PER_MSG 32

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

//!< ----- Prompts for read & write bytes -----
const char usr_msg_addr_prompt[] PROGMEM = {
        "Provide starting address\n(use 8-char hex ascii from range 00000000 - 0000FFFF, i.e. 0000ABCD): 0x"
};
const char usr_msg_no_bytes_to_read_prompt[] PROGMEM = {
        "Provide number of bytes to be read: "
};
const char usr_msg_no_bytes_to_write_prompt[] PROGMEM = {
        "Provide number of bytes to be written: "
};
// --------------------------------------------

#endif /* INCLUDE_USR_MSG_H_ */
