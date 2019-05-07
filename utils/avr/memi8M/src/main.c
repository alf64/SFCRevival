/*
 * main.c
 *
 *  Created on: 29 kwi 2019
 *      Author: bmudlaff
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usr_msg.h"
#include "Comm/comm.h"
#include "Ascii/ascii.h"
#include <boards/memi8M_pcb.h>

//!< Maximum user input (in bytes) from comm
#define MAX_USR_INPUT 8
//!< Maximum this system output for user (in bytes, including '\0' char)
#define MAX_SYS_OUTPUT 9
unsigned char usr_input[MAX_USR_INPUT];
unsigned char sys_output[MAX_SYS_OUTPUT];

int main(void)
{
    // turn ON interrupts
    sei();

    ascii_status_t ascii_status = ASCII_SUCCESS;

    // init communication (via UART) mechanism
    comm_status_t c_stat = CommInit(
            57600,
            F_CPU,
            NULL,
            0);
    if(c_stat != COMM_SUCCESS)
        while(1); // stuck here forever

    for(;;)
    {
        // display main menu
        c_stat = CommSendMsgsArrFromFlash(
                &usr_msg_main_menu[0][0],
                USR_MSG_MAIN_AVAILABLE_USR_MSGS,
                USR_MSG_MAIN_MAX_CHARS_PER_MSG);
        if(c_stat != COMM_SUCCESS)
            while(1){}; // halt

        // get user msg
        CommCleanMsgBuffer();
        do
        {
            c_stat = CommGetMsg(1, usr_input, sizeof(usr_input));
        } while(c_stat != COMM_SUCCESS);
        CommSendMsgFromFlash(usr_msg_input_received, sizeof(usr_msg_input_received-1));

        switch(usr_input[0])
        {
            case '1': // read-bytes option
            {
                // ----- get addr from usr -----
                CommSendMsgFromFlash(
                        usr_msg_addr_prompt,
                        sizeof(usr_msg_addr_prompt-1));
                CommCleanMsgBuffer();
                while(CommGetMsg(8, usr_input, sizeof(usr_input)) != COMM_SUCCESS);
                CommSendMsgFromFlash(usr_msg_input_received, sizeof(usr_msg_input_received-1));

                uint32_t addr;
                ascii_status = AsciiToU32(usr_input, &addr);
                if(ascii_status == ASCII_INVALID_RANGE)
                {
                    CommSendMsgFromFlash(
                            usr_msg_invalid_input_not_hex,
                            sizeof(usr_msg_invalid_input_not_hex-1));
                    break;
                }
                else if(ascii_status == ASCII_FAILED)
                {
                    CommSendMsgFromFlash(
                            usr_msg_critical_err,
                            sizeof(usr_msg_critical_err-1));
                    while(1){}; // stuck forever
                }

                // ----- get no of bytes from usr -----
                CommSendMsgFromFlash(
                        usr_msg_no_bytes_to_read_prompt,
                        sizeof(usr_msg_no_bytes_to_read_prompt-1));
                CommCleanMsgBuffer();
                while(CommGetMsg(8, usr_input, sizeof(usr_input)) != COMM_SUCCESS);
                CommSendMsgFromFlash(usr_msg_input_received, sizeof(usr_msg_input_received-1));

                uint32_t bts;
                ascii_status = AsciiToU32(usr_input, &bts);
                if(ascii_status == ASCII_INVALID_RANGE)
                {
                    CommSendMsgFromFlash(
                            usr_msg_invalid_input_not_hex,
                            sizeof(usr_msg_invalid_input_not_hex-1));
                    break;
                }
                else if(ascii_status == ASCII_FAILED)
                {
                    CommSendMsgFromFlash(
                            usr_msg_critical_err,
                            sizeof(usr_msg_critical_err-1));
                    while(1){}; // stuck forever
                }

                // ----- convert addr to ascii and display it to usr -----
                CommSendMsgFromFlash(
                        usr_msg_processing,
                        sizeof(usr_msg_processing-1));
                ascii_status = U32ToAscii(
                        addr,
                        sys_output,
                        sizeof(sys_output));
                if(ascii_status != ASCII_SUCCESS)
                {
                    CommSendMsgFromFlash(
                            usr_msg_critical_err,
                            sizeof(usr_msg_critical_err-1));
                    while(1){}; // stuck forever
                }
                CommSendMsgFromFlash(
                        usr_msg_given_addr_is,
                        sizeof(usr_msg_given_addr_is-1));
                CommSendMsg(
                        sys_output,
                        8);

                // ----- convert bts to ascii and display it to usr -----
                ascii_status = U32ToAscii(
                        bts,
                        sys_output,
                        sizeof(sys_output));
                if(ascii_status != ASCII_SUCCESS)
                {
                    CommSendMsgFromFlash(
                            usr_msg_critical_err,
                            sizeof(usr_msg_critical_err-1));
                    while(1){}; // stuck forever
                }
                CommSendMsgFromFlash(
                        usr_msg_given_no_bytes_is,
                        sizeof(usr_msg_given_no_bytes_is-1));
                CommSendMsg(
                        sys_output,
                        8);

                // ----- check the sanity of the addr and bytes and addr vs bytes -----
                if(addr > BOARD_MAX_ADDRESS)
                {
                    CommSendMsgFromFlash(
                            usr_msg_addr_out_of_range_err,
                            sizeof(usr_msg_addr_out_of_range_err-1));
                    break;
                }
                else if((bts > BOARD_SPACE_CAPACITY) || (bts == 0))
                {
                    CommSendMsgFromFlash(
                            usr_msg_bts_out_of_range_err,
                            sizeof(usr_msg_bts_out_of_range_err-1));
                    break;
                }
                else if(((addr + bts)-1) > BOARD_MAX_ADDRESS)
                {
                    CommSendMsgFromFlash(
                            usr_msg_addr_vs_bts_err,
                            sizeof(usr_msg_addr_vs_bts_err-1));
                    break;
                }

                // code for read bytes here
                break;
            }
            case '2':
            {
                // code for write bytes here
                break;
            }
            case '3':
            {
                // code for read all memory here
                break;
            }
            case '4':
            {
                // code for write all memory here
                break;
            }
            default:
            {
                CommSendMsgFromFlash(usr_msg_unsupported_sel, sizeof(usr_msg_unsupported_sel-1));
                break;
            }
        }

        // TODO: ask user to continue or halt forever
        while(1) {}; // halt
    }

    for(;;);

    return 0;
}
