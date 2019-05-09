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
#include "boards/memi8M_pcb.h"

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
            while(1){}; // halt forever

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
                uint32_t addr = 0;
                usr_msg_status_t usrmsg_status =
                        UsrMsgAskForAddr(usr_input, sizeof(usr_input), &addr);
                if(usrmsg_status == USR_MSG_FAILED)
                {
                    while(1){}; // critical error, stuck forever
                }
                else if(usrmsg_status == USR_MSG_INVALID_INPUT)
                {
                    break; // break this switch case
                }

                // ----- get no of bytes from usr -----
                uint32_t bts = 0;
                usrmsg_status =
                        UsrMsgAskForNoOfBts(usr_input, sizeof(usr_input), &bts);
                if(usrmsg_status == USR_MSG_FAILED)
                {
                    while(1){}; // critical error, stuck forever
                }
                else if(usrmsg_status == USR_MSG_INVALID_INPUT)
                {
                    break; // break this switch case
                }

                // ----- get output format from usr -----
                usr_out_fmt outfmt = USR_OUT_FMT_BYTES;
                usrmsg_status =
                        UsrMsgAskForOutFmt(usr_input, sizeof(usr_input), &outfmt);
                if(usrmsg_status == USR_MSG_FAILED)
                {
                    while(1){}; // critical error, stuck forever
                }
                else if(usrmsg_status == USR_MSG_INVALID_INPUT)
                {
                    break; // break this switch case
                }

                // ----- convert addr to ascii and display it to usr -----
                usrmsg_status =
                        UsrMsgDispAddrAsAscii(addr, sys_output, sizeof(sys_output));
                if(usrmsg_status == USR_MSG_FAILED)
                {
                    while(1){}; // critical error, stuck forever
                }

                // ----- convert bts to ascii and display it to usr -----
                usrmsg_status =
                        UsrMsgDispNoOfBtsAsAscii(bts, sys_output, sizeof(sys_output));
                if(usrmsg_status == USR_MSG_FAILED)
                {
                    while(1){}; // critical error, stuck forever
                }

                // ----- display output format to usr -----
                usrmsg_status = UsrMsgDispOutFmtAsAscii(outfmt);
                if(usrmsg_status == USR_MSG_FAILED)
                {
                    while(1){}; // critical error, stuck forever
                }

                // ----- check the sanity of the addr and bytes and addr vs bytes -----
                usrmsg_status = UsrMsgAddrBtsCheck(addr, bts);
                if(usrmsg_status == USR_MSG_INVALID_INPUT)
                {
                    break; // break this switch case
                }

                // ----- ask usr if he is sure to proceed with its choices -----
                CommSendMsgFromFlash(
                        usr_msg_proceed_prompt,
                        sizeof(usr_msg_proceed_prompt-1));
                CommCleanMsgBuffer();
                while(CommGetMsg(1, usr_input, sizeof(usr_input)) != COMM_SUCCESS);
                CommSendMsgFromFlash(usr_msg_input_received, sizeof(usr_msg_input_received-1));

                if((usr_input[0] != 'y') && (usr_input[0] != 'n')
                        && (usr_input[0] != 'Y') && (usr_input[0] != 'N'))
                {
                    CommSendMsgFromFlash(
                            usr_msg_unsupported_sel,
                            sizeof(usr_msg_unsupported_sel-1));
                    break;
                }
                else if((usr_input[0] == 'n') || (usr_input[0] == 'N'))
                {
                    break; // usr said no
                }

                uint8_t readbt = 0;
                for(uint32_t i = 0; i < bts; i++)
                {
                    /*
                     * code for read byte here
                     * up to this point you shall have the following vars set:
                     * addr - the address to be read
                     * bts - the number of bytes to be read
                     * outfmt - output format to use when giving read bytes to user via comm
                     */
                    // readbt = ReadByte(addr+i)

                    if(outfmt == USR_OUT_FMT_ASCII)
                    {
                        CommSendByteAsHexAscii(readbt);
                    }
                    else
                    {
                        CommSendBytes(&readbt, 1);
                    }
                }

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
