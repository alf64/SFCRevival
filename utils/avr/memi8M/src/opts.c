/*
 * opts.c
 *
 *  Created on: 10 maj 2019
 *      Author: bmudlaff
 */


#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Comm/comm.h"
#include "usr_msg.h"
#include "opts.h"


opts_status_t OptsReadBytes(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size)
{
    if((inp_buff == NULL) || (out_buff) == NULL)
        return OPTS_CRITICAL_ERR;

    if((inp_buff_size < 8) || (out_buff_size < 9))
        return OPTS_CRITICAL_ERR; // functions called below have such requirements for sizes

    // ----- get addr from usr -----
    uint32_t addr = 0;
    usr_msg_status_t usrmsg_status =
            UsrMsgAskForAddr(inp_buff, inp_buff_size, &addr);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    // ----- get no of bytes from usr -----
    uint32_t bts = 0;
    usrmsg_status =
            UsrMsgAskForNoOfBts(inp_buff, inp_buff_size, &bts);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    // ----- get output format from usr -----
    usr_out_fmt outfmt = USR_OUT_FMT_BYTES;
    usrmsg_status =
            UsrMsgAskForOutFmt(inp_buff, inp_buff_size, &outfmt);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    // ----- convert addr to ascii and display it to usr -----
    usrmsg_status =
            UsrMsgDispAddrAsAscii(addr, out_buff, out_buff_size);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }

    // ----- convert bts to ascii and display it to usr -----
    usrmsg_status =
            UsrMsgDispNoOfBtsAsAscii(bts, out_buff, out_buff_size);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }

    // ----- display output format to usr -----
    usrmsg_status = UsrMsgDispOutFmtAsAscii(outfmt);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }

    // ----- check the sanity of the addr and bytes and addr vs bytes -----
    usrmsg_status = UsrMsgAddrBtsCheck(addr, bts);
    if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    // ----- ask usr if he is sure to proceed with its choices -----
    uint8_t proceed = 0;
    usrmsg_status =
            UsrMsgAskForProceed(inp_buff, inp_buff_size, &proceed);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    if(!proceed)
    {
        return OPTS_NEED_RETRY;
    }

    uint8_t readbt = 0;
    for(uint32_t i = 0; i < bts; i++)
    {
        /*
         * TODO: code for read bytes here
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

    CommSendMsgFromFlash(
            usr_msg_job_done,
            sizeof(usr_msg_job_done-1));

    return OPTS_SUCCESS;
}
