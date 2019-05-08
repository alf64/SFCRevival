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

extern const char usr_msg_main_menu[USR_MSG_MAIN_AVAILABLE_USR_MSGS][USR_MSG_MAIN_MAX_CHARS_PER_MSG] PROGMEM;

extern const char usr_msg_unsupported_sel[] PROGMEM;

extern const char usr_msg_critical_err[] PROGMEM;

extern const char usr_msg_input_received[] PROGMEM;

extern const char usr_msg_invalid_input_not_hex[] PROGMEM;

extern const char usr_msg_processing[] PROGMEM;

extern const char usr_msg_proceed_prompt[] PROGMEM;

//!< ----- Prompts for read & write bytes -----
extern const char usr_msg_addr_prompt[] PROGMEM;
extern const char usr_msg_addr_fmt_advice[] PROGMEM;
extern const char usr_msg_no_bytes_prompt[] PROGMEM;
extern const char usr_msg_no_bytes_fmt_advice[] PROGMEM;
extern const char usr_msg_given_addr_is[] PROGMEM;
extern const char usr_msg_given_no_bytes_is[] PROGMEM;
extern const char usr_msg_addr_out_of_range_err[] PROGMEM;
extern const char usr_msg_bts_out_of_range_err[] PROGMEM;
extern const char usr_msg_addr_vs_bts_err[] PROGMEM;
extern const char usr_msg_output_format_prompt[] PROGMEM;
extern const char usr_msg_output_format_invalid_err[] PROGMEM;
extern const char usr_msg_given_output_format_is[] PROGMEM;
extern const char usr_msg_given_output_format_ascii[] PROGMEM;
extern const char usr_msg_given_output_format_bytes[] PROGMEM;

//!< Output format
typedef enum
{
    USR_OUT_FMT_BYTES = '0',
    USR_OUT_FMT_ASCII = '1'
}usr_out_fmt;

typedef enum
{
    USR_MSG_SUCCESS = 0,
    USR_MSG_FAILED = 1,
    USR_MSG_INVALID_INPUT = 2
}usr_msg_status_t;

/*
 * @brief Interactively (using comm) asks user for an address.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param usr_input_buff A pointer to a buffer meant for storing user input
 * given via comm.
 * @param usr_input_buff_size A size (in bytes) of the given usr_input_buff
 * This parameter cannot be less than 8.
 * @param addr A pointer where the result (obtained address) shall be placed.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to obtain the address from user.
 * @retval USR_MSG_FAILED Means the critical error occured.
 * @retval USR_MSG_INVALID_INPUT Means the user gave invalid input (addr is not obtained).
 */
usr_msg_status_t UsrMsgAskForAddr(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint32_t* addr);

/*
 * @brief Interactively (using comm) asks user for number of bytes.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param usr_input_buff A pointer to a buffer meant for storing user input
 * given via comm.
 * @param usr_input_buff_size A size (in bytes) of the given usr_input_buff
 * This parameter cannot be less than 8.
 * @param bts A pointer where the result (obtained number of bytes) shall be placed.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to obtain the number of bytes from user.
 * @retval USR_MSG_FAILED Means the critical error occured.
 * @retval USR_MSG_INVALID_INPUT Means the user gave invalid input (bts is not obtained).
 */
usr_msg_status_t UsrMsgAskForNoOfBts(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint32_t* bts);

#endif /* INCLUDE_USR_MSG_H_ */
