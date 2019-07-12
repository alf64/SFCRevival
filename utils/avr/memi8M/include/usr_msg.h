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
#include "boards/memi8M_pcb.h"

//!< main menu messages
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
#define USR_MSG_MAIN_AVAILABLE_USR_MSGS 13
#define USR_MSG_MAIN_MAX_CHARS_PER_MSG 32

//!< main menu choices
#define USR_MSG_READ_BYTES_CHOICE '1'
#define USR_MSG_WRITE_BYTES_CHOICE '2'
#define USR_MSG_READ_ALL_CHOICE '3'
#define USR_MSG_WRITE_ALL_CHOICE '4'
#define USR_MSG_ERASE_SECTOR_CHOICE '5'
#define USR_MSG_ERASE_BLOCK_CHOICE '6'
#define USR_MSG_ERASE_ALL_CHOICE '7'
#define USR_MSG_CHECK_PROD_ID_CHOICE '8'
#define USR_MSG_DBG_SC_01_CHOICE '9'
#define USR_MSG_DBG_SC_02_CHOICE 'A'

extern const char usr_msg_main_menu[USR_MSG_MAIN_AVAILABLE_USR_MSGS][USR_MSG_MAIN_MAX_CHARS_PER_MSG] PROGMEM;

extern const char usr_msg_unsupported_sel[23] PROGMEM;

extern const char usr_msg_critical_err[36] PROGMEM;

extern const char usr_msg_input_received[16] PROGMEM;

extern const char usr_msg_invalid_input_not_hex[26] PROGMEM;

extern const char usr_msg_proceed_prompt[24] PROGMEM;

extern const char usr_msg_retry_prompt[22] PROGMEM;

extern const char usr_msg_job_done[11] PROGMEM;

extern const char usr_msg_prog_to_restart[23] PROGMEM;

extern const char usr_msg_system_halted[16] PROGMEM;

//!< ----- Prompts for read & write bytes -----
extern const char usr_msg_addr_prompt[26] PROGMEM;
extern const char usr_msg_addr_fmt_advice[71] PROGMEM;
extern const char usr_msg_no_bytes_prompt[25] PROGMEM;
extern const char usr_msg_no_bytes_fmt_advice[71] PROGMEM;
extern const char usr_msg_given_addr_is[19] PROGMEM;
extern const char usr_msg_given_no_bytes_is[24] PROGMEM;
extern const char usr_msg_addr_out_of_range_err[35] PROGMEM;
extern const char usr_msg_bts_out_of_range_err[33] PROGMEM;
extern const char usr_msg_addr_vs_bts_err[40] PROGMEM;
extern const char usr_msg_data_format_prompt[44] PROGMEM;
extern const char usr_msg_data_format_invalid_err[34] PROGMEM;
extern const char usr_msg_given_data_format_is[23] PROGMEM;
extern const char usr_msg_data_format_ascii[6] PROGMEM;
extern const char usr_msg_data_format_bytes[6] PROGMEM;
extern const char usr_msg_data_ascii_prompt[43] PROGMEM;
extern const char usr_msg_data_raw_prompt[26] PROGMEM;
extern const char usr_msg_read_data_info[15] PROGMEM;
extern const char usr_msg_write_data_atten[83] PROGMEM;

//!< ----- Infos for read & write all -----
extern const char usr_msg_readall_info[215] PROGMEM;
extern const char usr_msg_writeall_info[240] PROGMEM;
extern const char usr_msg_memsize_is[31] PROGMEM;
extern const char usr_msg_all_data_prompt[31] PROGMEM;

//!< -----------Prompts for erase ------------
extern const char usr_msg_sector_addr_prompt[24] PROGMEM;
extern const char usr_msg_sector_addr_fmt_advice[71] PROGMEM;
extern const char usr_msg_block_addr_prompt[23] PROGMEM;
extern const char usr_msg_block_addr_fmt_advice[71] PROGMEM;
extern const char usr_msg_sector_addr_out_of_range_err[42] PROGMEM;
extern const char usr_msg_block_addr_out_of_range_err[41] PROGMEM;
extern const char usr_msg_eraseall_info[35] PROGMEM;
extern const char usr_msg_check_prod_id_info[34] PROGMEM;
extern const char usr_msg_man_id_is[18] PROGMEM;
extern const char usr_msg_dev_id_is[12] PROGMEM;
extern const char usr_msg_man_id_recognized_as[32] PROGMEM;
extern const char usr_msg_dev_id_recognized_as[26] PROGMEM;
extern const char usr_msg_man_id_unrecognized[31] PROGMEM;
extern const char usr_msg_dev_id_unrecognized[25] PROGMEM;
extern const char usr_msg_man_id_name[MEM_MANUFACTURER_NAME_LEN] PROGMEM;
extern const char usr_msg_dev_id_name[MEM_DEVICE_NAME_LEN] PROGMEM;

//!< Output format
typedef enum
{
    USR_DATA_FMT_BYTES = '0',
    USR_DATA_FMT_ASCII = '1'
}usr_data_fmt;

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
 * @brief Interactively (using comm) asks user for the sector address.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param usr_input_buff A pointer to a buffer meant for storing user input
 * given via comm.
 * @param usr_input_buff_size A size (in bytes) of the given usr_input_buff
 * This parameter cannot be less than 8.
 * @param sector_addr A pointer where the result (obtained sector address) shall be placed.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to obtain the sector address from user.
 * @retval USR_MSG_FAILED Means the critical error occured.
 * @retval USR_MSG_INVALID_INPUT Means the user gave invalid input (sector_addr is not obtained).
 */
usr_msg_status_t UsrMsgAskForSectorAddr(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint32_t* sector_addr);

/*
 * @brief Interactively (using comm) asks user for the block address.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param usr_input_buff A pointer to a buffer meant for storing user input
 * given via comm.
 * @param usr_input_buff_size A size (in bytes) of the given usr_input_buff
 * This parameter cannot be less than 8.
 * @param block_addr A pointer where the result (obtained block address) shall be placed.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to obtain the block address from user.
 * @retval USR_MSG_FAILED Means the critical error occured.
 * @retval USR_MSG_INVALID_INPUT Means the user gave invalid input (block_addr is not obtained).
 */
usr_msg_status_t UsrMsgAskForBlockAddr(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint32_t* block_addr);

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

/*
 * @brief Interactively (using comm) asks user for data format (ascii vs raw bytes).
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param usr_input_buff A pointer to a buffer meant for storing user input
 * given via comm.
 * @param usr_input_buff_size A size (in bytes) of the given usr_input_buff
 * This parameter cannot be less than 1.
 * @param bts A pointer where the result (obtained data format) shall be placed.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to obtain data format.
 * @retval USR_MSG_FAILED Means the critical error occurred.
 * @retval USR_MSG_INVALID_INPUT Means the user gave invalid input (data format is not obtained).
 */
usr_msg_status_t UsrMsgAskForDataFmt(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        usr_data_fmt* fmt);

/*
 * @brief Interactively (using comm) asks user if he wants to proceed further or no.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param usr_input_buff A pointer to a buffer meant for storing user input
 * given via comm.
 * @param usr_input_buff_size A size (in bytes) of the given usr_input_buff
 * This parameter cannot be less than 1.
 * @param proceed A pointer where the result (obtained user decision for proceeding) shall be placed.
 * The result (proceed) may have the following values:
 * 0 - means FALSE
 * 1 - means TRUE
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to obtain the proceed decision from user.
 * @retval USR_MSG_FAILED Means the critical error occurred.
 * @retval USR_MSG_INVALID_INPUT Means the user gave invalid input (proceed is not obtained).
 */
usr_msg_status_t UsrMsgAskForProceed(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint8_t* proceed);

/*
 * @brief Interactively (using comm) asks user if he wants to retry.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param usr_input_buff A pointer to a buffer meant for storing user input
 * given via comm.
 * @param usr_input_buff_size A size (in bytes) of the given usr_input_buff
 * This parameter cannot be less than 1.
 * @param retry A pointer where the result (obtained user decision for retrying) shall be placed.
 * The result (retry) may have the following values:
 * 0 - means FALSE
 * 1 - means TRUE
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to obtain the retry decision from user.
 * @retval USR_MSG_FAILED Means the critical error occurred.
 * @retval USR_MSG_INVALID_INPUT Means the user gave invalid input (retry is not obtained).
 */
usr_msg_status_t UsrMsgAskForRetry(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        uint8_t* retry);

/*
 * @brief Interactively (using comm) asks user for the single byte of data.
 *
 * @attention
 * This function relies on comm, so it is necessary to perform CommInit()
 * before using it.
 *
 * @param usr_input_buff A pointer to a buffer meant for storing user input
 * given via comm.
 * @param usr_input_buff_size A size (in bytes) of the given usr_input_buff
 * This parameter cannot be less than 2.
 * @param fmt A format which shall be used to obtain data from user.
 * @param data A pointer where the result (obtained byte of data) shall be placed.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to obtain data from user.
 * @retval USR_MSG_FAILED Means the critical error occurred.
 * @retval USR_MSG_INVALID_INPUT Means the user gave invalid input (data is not obtained).
 */
usr_msg_status_t UsrMsgAskForSingleData(
        unsigned char* usr_input_buff,
        uint8_t usr_input_buff_size,
        usr_data_fmt fmt,
        uint8_t* data);

/*
 * @brief Displays given addr to user via comm.
 *
 * @details
 * This function takes given addr, converts it internally to hex ascii and then
 * sends it via comm to display it to user.
 *
 * @param addr An address to be displayed.
 * @param workbuff A pointer to a work buffer which is needed by this function for:
 * - u32 to hex ascii conversion purposes
 * - displaying the message via comm
 * @param workbuff_size A size (in bytes) of the given workbuff.
 * This shall be no less than 9.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to display the addr.
 * @retval USR_MSG_FAILED Means the critical error occurred.
 */
usr_msg_status_t UsrMsgDispAddrAsAscii(
        uint32_t addr,
        unsigned char* workbuff,
        uint8_t workbuff_size);

/*
 * @brief Displays given bts ("number of bytes") to user via comm.
 *
 * @details
 * This function takes given bts, converts it internally to hex ascii and then
 * sends it via comm to display it to user.
 *
 * @param bts "number of bytes" to be displayed.
 * @param workbuff A pointer to a work buffer which is needed by this function for:
 * - u32 to hex ascii conversion purposes
 * - displaying the message via comm
 * @param workbuff_size A size (in bytes) of the given workbuff.
 * This shall be no less than 9.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to display the bts.
 * @retval USR_MSG_FAILED Means the critical error occurred.
 */
usr_msg_status_t UsrMsgDispNoOfBtsAsAscii(
        uint32_t bts,
        unsigned char* workbuff,
        uint8_t workbuff_size);

/*
 * @brief Displays given fmt ("output format") to user via comm.
 *
 * @details
 * This function takes given fmt, interprets it and
 * sends appropriate message via comm to display it to user.
 *
 * @param fmt "output format" to be displayed.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to display the fmt.
 * @retval USR_MSG_FAILED Means the critical error occurred.
 */
usr_msg_status_t UsrMsgDispDataFmtAsAscii(usr_data_fmt fmt);

/*
 * @brief Displays given man_id (manufacturer id) and dev_id (device id) to user via comm.
 *
 * @details
 * This function takes given man_id and dev_id, converts it internally to hex ascii and then
 * sends it via comm to display it to user.
 *
 * @param man_id Manufacturer Id to be displayed.
 * @param dev_id Device Id to be displayed.
 * @param workbuff A pointer to a work buffer which is needed by this function for:
 * - u8 to hex ascii conversion purposes
 * - displaying the message via comm
 * @param workbuff_size A size (in bytes) of the given workbuff.
 * This shall be no less than 3.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to display the man_id and dev_id.
 * @retval USR_MSG_FAILED Means the critical error occurred.
 */
usr_msg_status_t UsrMsgDispProdIdAsAscii(
        uint8_t man_id,
        uint8_t dev_id,
        unsigned char* workbuff,
        uint8_t workbuff_size);

/*
 * @brief Displays progress to user via comm.
 *
 * @details
 * This function takes current and limit value, converts them into hex ascii and sends
 * appropriate information to user via comm.
 *
 * @param bts current A current value.
 * @param limit A limit value.
 * @param workbuff A pointer to a work buffer which is needed by this function for:
 * - u32 to hex ascii conversion purposes
 * - displaying the message via comm
 * @param workbuff_size A size (in bytes) of the given workbuff.
 * This shall be no less than 9.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to display the bts.
 * @retval USR_MSG_INVALID_INPUT Invalid input arguments were given.
 * @retval USR_MSG_FAILED Means the critical error occurred.
 */
usr_msg_status_t UsrMsgDispProgress(
        uint32_t current,
        uint32_t limit,
        unsigned char* workbuff,
        uint8_t workbuff_size);

/*
 * @brief Checks the sanity of the addr and bts.
 *
 * @details This function takes addr and bts ("number of bytes") and checks if
 * their values are sane. If there is a problem with them, sends appropriate message to
 * user informing him about it.
 *
 * @param addr Address to be checked.
 * @param bts "Number of bytes" to be checked.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the given addr and bts are sane.
 * @retval USR_MSG_INVALID_INPUT Means the given addr or bts is (are) not sane.
 */
usr_msg_status_t UsrMsgAddrBtsCheck(
        uint32_t addr,
        uint32_t bts);

/*
 * @brief Checks the sanity of the sector_addr.
 *
 * @details This function takes sector_addr and checks if
 * its value is sane. If there is a problem with it, sends appropriate message to
 * user informing him about it.
 *
 * @param sector_addr Sector address to be checked.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the given sector_addr is sane.
 * @retval USR_MSG_INVALID_INPUT Means the given sector_addr is not sane.
 */
usr_msg_status_t UsrMsgSectorAddrCheck(uint32_t sector_addr);

/*
 * @brief Checks the sanity of the block_addr.
 *
 * @details This function takes block_addr and checks if
 * its value is sane. If there is a problem with it, sends appropriate message to
 * user informing him about it.
 *
 * @param block_addr Block address to be checked.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the given block_addr is sane.
 * @retval USR_MSG_INVALID_INPUT Means the given block_addr is not sane.
 */
usr_msg_status_t UsrMsgBlockAddrCheck(uint32_t block_addr);

/*
 * @brief Checks the sanity of the man_id (manufacturer id) and dev_id (device id).
 *
 * @details This function takes man_id and dev_id then checks if
 * their values are sane. If there is a problem with them, sends appropriate message to
 * user informing him about it.
 *
 * @param man_id Manufacturer Id to be checked.
 * @param dev_id Device Id to be checked.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the given man_id and dev_id are sane.
 * @retval USR_MSG_INVALID_INPUT Means the given man_id and dev_id are not sane.
 */
usr_msg_status_t UsrMsgProdIdCheck(
        uint8_t man_id,
        uint8_t dev_id);

/*
 * @brief Displays information to user about read-all behavior.
 *
 * @param mem_size Total size of the memory that's going to be read.
 * @param workbuff A pointer to a work buffer which is needed by this function for:
 * - u32 to hex ascii conversion purposes
 * - displaying the message via comm
 * @param workbuff_size A size (in bytes) of the given workbuff.
 * This shall be no less than 9.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to display the info.
 * @retval USR_MSG_FAILED Means the critical error occurred.
 */
usr_msg_status_t UsrMsgDispReadAllInfo(
        uint32_t mem_size,
        unsigned char* workbuff,
        uint8_t workbuff_size);

/*
 * @brief Displays information to user about write-all behavior.
 *
 * @param mem_size Total size of the memory that's going to be written.
 * @param workbuff A pointer to a work buffer which is needed by this function for:
 * - u32 to hex ascii conversion purposes
 * - displaying the message via comm
 * @param workbuff_size A size (in bytes) of the given workbuff.
 * This shall be no less than 9.
 *
 * @returns usr_msg_status_t
 * @retval USR_MSG_SUCCESS Means the function succeeded to display the info.
 * @retval USR_MSG_FAILED Means the critical error occurred.
 */
usr_msg_status_t UsrMsgDispWriteAllInfo(
        uint32_t mem_size,
        unsigned char* workbuff,
        uint8_t workbuff_size);


#endif /* INCLUDE_USR_MSG_H_ */
