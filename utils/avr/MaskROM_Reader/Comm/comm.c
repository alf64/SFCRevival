/*
 * comm.c
 *
 *  Created on: 15 kwi 2018
 *      Author: alf64
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "../USARTdrv/usart.h"
#include "comm.h"


char rcvbuff[COMM_RECEIVE_BUFFER_SIZE];
char* rcvmsg_start = rcvbuff;
char* rcvmsg_end = rcvbuff;

comm_status_t CommInit(
        uint32_t baud_rate,
        uint32_t f_cpu,
        const char* init_msg,
        uint8_t init_msg_size)
{
    comm_status_t status = COMM_SUCCESS;

    switch(baud_rate)
    {
        case 19200:
        {
            uint16_t ubrr = 0;
            uint32_t rest = (f_cpu % (8*baud_rate));
            if(rest > 0)
                return COMM_FAILED; // error rate > 0 not supported
            else
            {
                ubrr = (uint16_t)((f_cpu / (8*baud_rate)) - 1);
                UsartInit(ubrr); // ubrr = 71 for 19200bps and f_cpu: 11.0592 Mhz
            }
            break;
        }
        default:
        {
            return COMM_FAILED; // unsupported baud rate with given f_cpu
        }
    }

    if(init_msg == NULL)
        CommSendMsgFromFlash(defaultmsg, (sizeof(defaultmsg)-1));
    else
        CommSendMsg(init_msg, init_msg_size);

    return status;
}


comm_status_t CommSendMsg(
        const char* msg,
        uint8_t msg_size)
{
    comm_status_t status = COMM_SUCCESS;

    if((msg == NULL) || (msg_size == 0))
        return COMM_FAILED;

    for(uint8_t i = 0; i < msg_size; i++)
    {
        UsartTransmit((uint8_t)msg[i]);
    }

    // transmit new line character
    UsartTransmit(0x0a);

    return status;
}

comm_status_t CommSendMsgFromFlash(
        const char* PROGMEM msg,
        uint8_t msg_size)
{
    comm_status_t status = COMM_SUCCESS;

    if((msg == NULL) || (msg_size == 0))
        return COMM_FAILED;

    for(uint8_t i = 0; i < msg_size; i++)
    {
        UsartTransmit((uint8_t)pgm_read_byte(&msg[i]));
    }

    // transmit new line character
    UsartTransmit(0x0a);

    return status;
}

comm_status_t CommGetMsg(
        uint8_t msg_size,
        char* msg)
{
    comm_status_t status = COMM_SUCCESS;

    if(rcvmsg_end == NULL || rcvmsg_end == NULL || msg == NULL)
        return COMM_FAILED;

    uint8_t rcvmsg_len = rcvmsg_end - rcvmsg_start;

    if(rcvmsg_len >= msg_size)
    {
        // it's our message... Copy it!
        for(uint8_t i = 0; i < msg_size; i++)
        {
            *msg++ = *rcvmsg_start++;
        }

        // restart rcv buffer. If the buffer has more bytes: we don't care and fry them
        rcvmsg_start = rcvbuff;
        rcvmsg_end = rcvbuff;
    }
    else if(rcvmsg_len < msg_size)
        return COMM_RECEIVE_MSG_NOT_READY;
    else if(rcvmsg_len == 0)
        return COMM_RECEIVE_BUFF_EMPTY;
    else
    {
        // dunno what to do here ...
        return COMM_FAILED;
    }

    return status;
}


// isr upon a receive on UART...
ISR(USART_RXC_vect)
{
    if(rcvmsg_start != NULL && rcvmsg_end != NULL
            && rcvmsg_end < rcvbuff + sizeof(rcvbuff))
    {
        *rcvmsg_end++ = UDR;
    }
    else
    {
        // rcvbuff may be full, do nothing. Someone needs to read rcvbuff.
    }
}
