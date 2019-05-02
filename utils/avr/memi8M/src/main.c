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

//!< Maximum user input (in bytes) from comm
#define MAX_USR_INPUT 8

int main(void)
{
    unsigned char usr_input[MAX_USR_INPUT];

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
            case '1':
            {
                CommSendMsgFromFlash(usr_msg_addr_prompt, sizeof(usr_msg_addr_prompt-1));
                CommCleanMsgBuffer();
                while(CommGetMsg(6, usr_input, sizeof(usr_input)) != COMM_SUCCESS);

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

        while(1) {}; // halt
    }

    for(;;);

    return 0;
}
