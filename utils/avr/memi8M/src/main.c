/*
 * main.c
 *
 *  Created on: 29 kwi 2019
 *      Author: bmudlaff
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Comm/comm.h"
#include "usr_msg.h"
#include "opts.h"
#include "sst.h"

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

    SSTInit(); // init mem interface

    for(;;)
    {
        SSTRet();

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
        CommSendMsgFromFlash(usr_msg_input_received, (sizeof(usr_msg_input_received)-1), 1);

        switch(usr_input[0])
        {
            case USR_MSG_READ_BYTES_CHOICE:
            {
                uint8_t retry = 0;
                do
                {
                    opts_status_t opts_status =
                            OptsReadBytes(usr_input, sizeof(usr_input), sys_output, sizeof(sys_output));
                    if(opts_status == OPTS_CRITICAL_ERR)
                    {
                        while(1){}; // halt forever
                    }
                    else // opts_status == OPTS_SUCCESS || opts_status == OPTS_NEED_RETRY
                    {
                        usr_msg_status_t usrmsg_status =
                                UsrMsgAskForRetry(usr_input, sizeof(usr_input), &retry);
                        if(usrmsg_status == USR_MSG_FAILED)
                        {
                            while(1){}; // critical, halt forever
                        }
                        else if(usrmsg_status == USR_MSG_INVALID_INPUT)
                        {
                            retry = 0; // retry not obtained, assume no retry
                        }
                        else // USR_MSG_SUCCESS
                        {
                            // retry obtained, do nothing, rely on its current value
                        }
                    }
                } while(retry);

                break;
            }
            case USR_MSG_WRITE_BYTES_CHOICE:
            {
                uint8_t retry = 0;
                do
                {
                    opts_status_t opts_status =
                            OptsWriteBytes(usr_input, sizeof(usr_input), sys_output, sizeof(sys_output));
                    if(opts_status == OPTS_CRITICAL_ERR)
                    {
                        while(1){}; // halt forever
                    }
                    else // opts_status == OPTS_SUCCESS || opts_status == OPTS_NEED_RETRY
                    {
                        usr_msg_status_t usrmsg_status =
                                UsrMsgAskForRetry(usr_input, sizeof(usr_input), &retry);
                        if(usrmsg_status == USR_MSG_FAILED)
                        {
                            while(1){}; // critical, halt forever
                        }
                        else if(usrmsg_status == USR_MSG_INVALID_INPUT)
                        {
                            retry = 0; // retry not obtained, assume no retry
                        }
                        else // USR_MSG_SUCCESS
                        {
                            // retry obtained, do nothing, rely on its current value
                        }
                    }
                } while(retry);

                break;
            }
            case USR_MSG_READ_ALL_CHOICE:
            {
                uint8_t retry = 0;
                do
                {
                    opts_status_t opts_status =
                            OptsReadAll(usr_input, sizeof(usr_input), sys_output, sizeof(sys_output));
                    if(opts_status == OPTS_NEED_RETRY)
                    {
                        usr_msg_status_t usrmsg_status =
                                UsrMsgAskForRetry(usr_input, sizeof(usr_input), &retry);
                        if(usrmsg_status == USR_MSG_FAILED)
                        {
                            while(1){}; // critical, halt forever
                        }
                        else if(usrmsg_status == USR_MSG_INVALID_INPUT)
                        {
                            retry = 0; // retry not obtained, assume no retry
                        }
                        else // USR_MSG_SUCCESS
                        {
                            // retry obtained, do nothing, rely on its current value
                        }
                    }
                    else //((opts_status == OPTS_CRITICAL_ERR) || (opts_status == OPTS_SUCCESS))
                    {
                        while(1){}; // SUCCESS or ERROR, halt forever
                    }
                } while(retry);

                break;
            }
            case USR_MSG_WRITE_ALL_CHOICE:
            {
                uint8_t retry = 0;
                do
                {
                    opts_status_t opts_status =
                            OptsWriteAll(usr_input, sizeof(usr_input), sys_output, sizeof(sys_output));
                    if(opts_status == OPTS_CRITICAL_ERR)
                    {
                        while(1){}; // halt forever
                    }
                    else // opts_status == OPTS_SUCCESS || opts_status == OPTS_NEED_RETRY
                    {
                        usr_msg_status_t usrmsg_status =
                                UsrMsgAskForRetry(usr_input, sizeof(usr_input), &retry);
                        if(usrmsg_status == USR_MSG_FAILED)
                        {
                            while(1){}; // critical, halt forever
                        }
                        else if(usrmsg_status == USR_MSG_INVALID_INPUT)
                        {
                            retry = 0; // retry not obtained, assume no retry
                        }
                        else // USR_MSG_SUCCESS
                        {
                            // retry obtained, do nothing, rely on its current value
                        }
                    }
                } while(retry);

                break;
            }
            case USR_MSG_ERASE_SECTOR_CHOICE:
            {
                uint8_t retry = 0;
                do
                {
                    opts_status_t opts_status =
                            OptsEraseSector(usr_input, sizeof(usr_input), sys_output, sizeof(sys_output));
                    if(opts_status == OPTS_CRITICAL_ERR)
                    {
                        while(1){}; // halt forever
                    }
                    else // opts_status == OPTS_SUCCESS || opts_status == OPTS_NEED_RETRY
                    {
                        usr_msg_status_t usrmsg_status =
                                UsrMsgAskForRetry(usr_input, sizeof(usr_input), &retry);
                        if(usrmsg_status == USR_MSG_FAILED)
                        {
                            while(1){}; // critical, halt forever
                        }
                        else if(usrmsg_status == USR_MSG_INVALID_INPUT)
                        {
                            retry = 0; // retry not obtained, assume no retry
                        }
                        else // USR_MSG_SUCCESS
                        {
                            // retry obtained, do nothing, rely on its current value
                        }
                    }
                } while(retry);

                break;
            }
            case USR_MSG_ERASE_BLOCK_CHOICE:
            {
                uint8_t retry = 0;
                do
                {
                    opts_status_t opts_status =
                            OptsEraseBlock(usr_input, sizeof(usr_input), sys_output, sizeof(sys_output));
                    if(opts_status == OPTS_CRITICAL_ERR)
                    {
                        while(1){}; // halt forever
                    }
                    else // opts_status == OPTS_SUCCESS || opts_status == OPTS_NEED_RETRY
                    {
                        usr_msg_status_t usrmsg_status =
                                UsrMsgAskForRetry(usr_input, sizeof(usr_input), &retry);
                        if(usrmsg_status == USR_MSG_FAILED)
                        {
                            while(1){}; // critical, halt forever
                        }
                        else if(usrmsg_status == USR_MSG_INVALID_INPUT)
                        {
                            retry = 0; // retry not obtained, assume no retry
                        }
                        else // USR_MSG_SUCCESS
                        {
                            // retry obtained, do nothing, rely on its current value
                        }
                    }
                } while(retry);

                break;
            }
            case USR_MSG_ERASE_ALL_CHOICE:
            {
                uint8_t retry = 0;
                do
                {
                    opts_status_t opts_status =
                            OptsEraseAll(usr_input, sizeof(usr_input));
                    if(opts_status == OPTS_CRITICAL_ERR)
                    {
                        while(1){}; // halt forever
                    }
                    else // opts_status == OPTS_SUCCESS || opts_status == OPTS_NEED_RETRY
                    {
                        usr_msg_status_t usrmsg_status =
                                UsrMsgAskForRetry(usr_input, sizeof(usr_input), &retry);
                        if(usrmsg_status == USR_MSG_FAILED)
                        {
                            while(1){}; // critical, halt forever
                        }
                        else if(usrmsg_status == USR_MSG_INVALID_INPUT)
                        {
                            retry = 0; // retry not obtained, assume no retry
                        }
                        else // USR_MSG_SUCCESS
                        {
                            // retry obtained, do nothing, rely on its current value
                        }
                    }
                } while(retry);

                break;
            }
            case USR_MSG_CHECK_PROD_ID_CHOICE:
            {
                uint8_t retry = 0;
                do
                {
                    opts_status_t opts_status =
                            OptsCheckProdId(usr_input, sizeof(usr_input), sys_output, sizeof(sys_output));
                    if(opts_status == OPTS_CRITICAL_ERR)
                    {
                        while(1){}; // halt forever
                    }
                    else // opts_status == OPTS_SUCCESS || opts_status == OPTS_NEED_RETRY
                    {
                        usr_msg_status_t usrmsg_status =
                                UsrMsgAskForRetry(usr_input, sizeof(usr_input), &retry);
                        if(usrmsg_status == USR_MSG_FAILED)
                        {
                            while(1){}; // critical, halt forever
                        }
                        else if(usrmsg_status == USR_MSG_INVALID_INPUT)
                        {
                            retry = 0; // retry not obtained, assume no retry
                        }
                        else // USR_MSG_SUCCESS
                        {
                            // retry obtained, do nothing, rely on its current value
                        }
                    }
                } while(retry);

                break;
            }
            default:
            {
                CommSendMsgFromFlash(usr_msg_unsupported_sel, (sizeof(usr_msg_unsupported_sel)-1), 1);
                break;
            }
        }

        CommSendMsgFromFlash(
                usr_msg_prog_to_restart,
                (sizeof(usr_msg_prog_to_restart)-1),
                1);
        uint8_t proceed = 0;
        usr_msg_status_t usrmsg_status =
                UsrMsgAskForProceed(usr_input, sizeof(usr_input), &proceed);
        if(usrmsg_status == USR_MSG_FAILED)
        {
            while(1){}; // critical error, stuck forever
        }
        else // usrmsg_status == USR_MSG_INVALID_INPUT || usrmsg_status == USR_MSG_SUCCESS
        {
            if(!proceed)
            {
                CommSendMsgFromFlash(
                        usr_msg_system_halted,
                        (sizeof(usr_msg_system_halted)-1),
                        1);
                while(1){};; // user said no, halt forever
            }
            // do nothing, restarting the program main loop
        }

    } // main for(;;)

    for(;;);

    return 0;
}
