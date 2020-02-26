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
#include "boards/memi8M-01_pcb.h"
#include "sst.h"


/*
 * @brief Gets addr, bts, fmt from usr via comm.
 * @param inp_buff A pointer to buffer which will be used for obtaining user input via comm.
 * @param inp_buff_size A size (in bytes) of the given inp_buff.
 * @param out_buff A pointer to buffer which will be used for giving output to user via comm.
 * @param out_buff_size A size (in bytes) of the given out_buff.
 * @param addr A pointer where the obtained addr shall be stored.
 * @param bts A pointer where the obtained bts shall be stored.
 * @param fmt A pointer where the obtained fmt shall be stored.
 * @returns opts_status_t
 * @retval OPTS_SUCCESS If succeeded to obtain all of the params.
 * @retval OPTS_CRITICAL_ERR If critcal error occurred (params not obtained).
 * @retval OPTS_NEED_RETRY If re-call of the function is needed since usr gave invalid input (params not obtained).
 */
opts_status_t GetAddrBtsFmt(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size,
        uint32_t* addr,
        uint32_t* bts,
        usr_data_fmt* fmt)
{
    if((inp_buff == NULL) || (out_buff) == NULL)
        return OPTS_CRITICAL_ERR;

    if((inp_buff_size < 8) || (out_buff_size < 9))
        return OPTS_CRITICAL_ERR; // functions called below have such requirements for sizes

    // ----- get addr from usr -----
    usr_msg_status_t usrmsg_status =
            UsrMsgAskForAddr(inp_buff, inp_buff_size, addr);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    // ----- get no of bytes from usr -----
    usrmsg_status =
            UsrMsgAskForNoOfBts(inp_buff, inp_buff_size, bts);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    // ----- get data format from usr -----
    usrmsg_status =
            UsrMsgAskForDataFmt(inp_buff, inp_buff_size, fmt);
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
            UsrMsgDispAddrAsAscii(*addr, out_buff, out_buff_size);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }

    // ----- convert bts to ascii and display it to usr -----
    usrmsg_status =
            UsrMsgDispNoOfBtsAsAscii(*bts, out_buff, out_buff_size);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }

    // ----- display data format to usr -----
    usrmsg_status = UsrMsgDispDataFmtAsAscii(*fmt);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }

    // ----- check the sanity of the addr and bytes and addr vs bytes -----
    usrmsg_status = UsrMsgAddrBtsCheck(*addr, *bts);
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

    return OPTS_SUCCESS;
}

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

    uint32_t addr = 0;
    uint32_t bts = 0;
    usr_data_fmt fmt = USR_DATA_FMT_ASCII;;

    opts_status_t opts_status =
            GetAddrBtsFmt(inp_buff,
                    inp_buff_size,
                    out_buff,
                    out_buff_size,
                    &addr,
                    &bts,
                    &fmt);
    if(opts_status != OPTS_SUCCESS)
        return opts_status;

    CommSendMsgFromFlash(
            usr_msg_read_data_info,
            (sizeof(usr_msg_read_data_info)-1),
            0);

    uint8_t readbt = 0;
    for(uint32_t i = 0; i < bts; i++)
    {
        sst_ec_t sst_status = SSTRead((addr+i), &readbt);
        if(sst_status != SST_SUCCESS)
        {
            CommSendMsgFromFlash(
                    usr_msg_critical_err,
                    (sizeof(usr_msg_critical_err)-1),
                    1);
            return OPTS_CRITICAL_ERR;
        }

        if(fmt == USR_DATA_FMT_ASCII)
        {
            CommSendByteAsHexAscii(readbt);
        }
        else
        {
            CommSendBytes(&readbt, 1);
        }
    }

    CommSendMsg((unsigned const char*)"\n", 1, 0);
    CommSendMsgFromFlash(
            usr_msg_job_done,
            (sizeof(usr_msg_job_done)-1),
            1);

    return OPTS_SUCCESS;
}

opts_status_t OptsWriteBytes(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size)
{
    if((inp_buff == NULL) || (out_buff) == NULL)
        return OPTS_CRITICAL_ERR;

    if((inp_buff_size < 8) || (out_buff_size < 9))
        return OPTS_CRITICAL_ERR; // functions called below have such requirements for sizes

    uint32_t addr = 0;
    uint32_t bts = 0;
    usr_data_fmt fmt = USR_DATA_FMT_ASCII;;

    opts_status_t opts_status =
            GetAddrBtsFmt(inp_buff,
                    inp_buff_size,
                    out_buff,
                    out_buff_size,
                    &addr,
                    &bts,
                    &fmt);
    if(opts_status != OPTS_SUCCESS)
        return opts_status;

    CommSendMsgFromFlash(
            usr_msg_write_data_atten,
            (sizeof(usr_msg_write_data_atten)-1),
            1);

    uint8_t writebt = 0;
    usr_msg_status_t usrmsg_status = USR_MSG_INVALID_INPUT;
    for(uint32_t i = 0; i < bts; i++)
    {
        // try to get byte of data as long as user correctly inputs it
        do
        {
            usrmsg_status =
                    UsrMsgAskForSingleData(
                            inp_buff,
                            inp_buff_size,
                            fmt,
                            &writebt);
        }while(usrmsg_status == USR_MSG_INVALID_INPUT);
        if(usrmsg_status != USR_MSG_SUCCESS)
        {
            return OPTS_CRITICAL_ERR;
        }

        SSTWrite(addr+i, writebt);

        usrmsg_status =
                UsrMsgDispProgress(
                        (i+1),
                        bts,
                        out_buff,
                        out_buff_size);
        if(usrmsg_status != USR_MSG_SUCCESS)
        {
            return OPTS_CRITICAL_ERR;
        }

    }

    CommSendMsgFromFlash(
            usr_msg_job_done,
            (sizeof(usr_msg_job_done)-1),
            1);

    return OPTS_SUCCESS;
}

opts_status_t OptsReadAll(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size)
{
    if((inp_buff == NULL) || (out_buff) == NULL)
        return OPTS_CRITICAL_ERR;

    if((inp_buff_size < 1) || (out_buff_size < 9))
        return OPTS_CRITICAL_ERR; // functions called below have such requirements for sizes

    usr_memcap_t memcap = USR_MEMCAP_8Mb;
    usr_msg_status_t usrmsg_status =
            UsrMsgAskForMemoryCapacity(
                    inp_buff,
                    inp_buff_size,
                    &memcap);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    uint32_t mem_size = 0;
    if(memcap == USR_MEMCAP_16Mb)
    {
        mem_size = 0x200000;
    }
    else
    {
        mem_size = 0x100000;
    }
    if(mem_size > BOARD_SPACE_CAPACITY)
    {
        return OPTS_CRITICAL_ERR;
    }

    usrmsg_status =
            UsrMsgDispReadAllInfo(
                    mem_size,
                    out_buff,
                    out_buff_size);
    if(usrmsg_status != USR_MSG_SUCCESS)
        return OPTS_CRITICAL_ERR;

    // ----- ask usr if he is sure to proceed -----
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
    for(uint32_t i = 0; i < BOARD_SPACE_CAPACITY; i++)
    {
        sst_ec_t sst_status = SSTRead(i, &readbt);
        if(sst_status != SST_SUCCESS)
        {
            CommSendMsgFromFlash(
                    usr_msg_critical_err,
                    (sizeof(usr_msg_critical_err)-1),
                    1);
            return OPTS_CRITICAL_ERR;
        }

        CommSendBytes(&readbt, 1); // give raw bytes to usr
    }

    return OPTS_SUCCESS;
}

opts_status_t OptsWriteAll(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size)
{
    if((inp_buff == NULL) || (out_buff) == NULL)
        return OPTS_CRITICAL_ERR;

    if((inp_buff_size < 1) || (out_buff_size < 9))
        return OPTS_CRITICAL_ERR; // functions called below have such requirements for sizes

    usr_memcap_t memcap = USR_MEMCAP_8Mb;
    usr_msg_status_t usrmsg_status =
            UsrMsgAskForMemoryCapacity(
                    inp_buff,
                    inp_buff_size,
                    &memcap);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    uint32_t mem_size = 0;
    if(memcap == USR_MEMCAP_16Mb)
    {
        mem_size = 0x200000;
    }
    else
    {
        mem_size = 0x100000;
    }
    if(mem_size > BOARD_SPACE_CAPACITY)
    {
        return OPTS_CRITICAL_ERR;
    }

    usrmsg_status =
            UsrMsgDispWriteAllInfo(
                    mem_size,
                    out_buff,
                    out_buff_size);
    if(usrmsg_status != USR_MSG_SUCCESS)
        return OPTS_CRITICAL_ERR;

    // ----- ask usr if he is sure to proceed -----
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

    CommSendMsgFromFlash(
            usr_msg_all_data_prompt,
            (sizeof(usr_msg_all_data_prompt)-1),
            1);
    CommCleanMsgBuffer();

    uint8_t writebt = 0;
    for(uint32_t i = 0; i < BOARD_SPACE_CAPACITY; i++)
    {
        while(CommGetMsg(sizeof(writebt), &writebt, sizeof(writebt)) != COMM_SUCCESS);
        SSTWrite(i, writebt);
    }

    CommSendMsgFromFlash(
            usr_msg_job_done,
            (sizeof(usr_msg_job_done)-1),
            1);

    return OPTS_SUCCESS;
}

opts_status_t OptsEraseSector(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size)
{
    if((inp_buff == NULL) || (out_buff) == NULL)
        return OPTS_CRITICAL_ERR;

    if((inp_buff_size < 8) || (out_buff_size < 9))
        return OPTS_CRITICAL_ERR; // functions called below have such requirements for sizes

    uint32_t sector_addr = 0;
    usr_msg_status_t usrmsg_status =
            UsrMsgAskForSectorAddr(
                    inp_buff,
                    inp_buff_size,
                    &sector_addr);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    usrmsg_status = UsrMsgSectorAddrCheck(sector_addr);
    if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    usrmsg_status = UsrMsgDispAddrAsAscii(
            sector_addr,
            out_buff,
            out_buff_size);
    if(usrmsg_status == USR_MSG_FAILED)
        return OPTS_CRITICAL_ERR;

    uint8_t proceed = 0;
    usrmsg_status = UsrMsgAskForProceed(
            inp_buff,
            inp_buff_size,
            &proceed);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    if(!proceed)
        return OPTS_NEED_RETRY;

    sst_ec_t sst_status = SSTEraseSector(sector_addr);
    if(sst_status != SST_SUCCESS)
    {
        return OPTS_CRITICAL_ERR;
    }

    CommSendMsgFromFlash(
            usr_msg_job_done,
            (sizeof(usr_msg_job_done)-1),
            1);

    return OPTS_SUCCESS;
}

opts_status_t OptsEraseBlock(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size)
{
    if((inp_buff == NULL) || (out_buff) == NULL)
        return OPTS_CRITICAL_ERR;

    if((inp_buff_size < 8) || (out_buff_size < 9))
        return OPTS_CRITICAL_ERR; // functions called below have such requirements for sizes

    uint32_t block_addr = 0;
    usr_msg_status_t usrmsg_status =
            UsrMsgAskForBlockAddr(
                    inp_buff,
                    inp_buff_size,
                    &block_addr);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    usrmsg_status = UsrMsgBlockAddrCheck(block_addr);
    if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    usrmsg_status = UsrMsgDispAddrAsAscii(
            block_addr,
            out_buff,
            out_buff_size);
    if(usrmsg_status == USR_MSG_FAILED)
        return OPTS_CRITICAL_ERR;

    uint8_t proceed = 0;
    usrmsg_status = UsrMsgAskForProceed(
            inp_buff,
            inp_buff_size,
            &proceed);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    if(!proceed)
        return OPTS_NEED_RETRY;

    sst_ec_t sst_status = SSTEraseBlock(block_addr);
    if(sst_status != SST_SUCCESS)
    {
        return OPTS_CRITICAL_ERR;
    }

    CommSendMsgFromFlash(
            usr_msg_job_done,
            (sizeof(usr_msg_job_done)-1),
            1);

    return OPTS_SUCCESS;
}

opts_status_t OptsEraseAll(
        unsigned char* inp_buff,
        uint32_t inp_buff_size)
{
    if((inp_buff == NULL) || (inp_buff_size < 1))
        return OPTS_CRITICAL_ERR;

    CommSendMsgFromFlash(
            usr_msg_eraseall_info,
            (sizeof(usr_msg_eraseall_info)-1),
            1);

    uint8_t proceed;
    usr_msg_status_t usrmsg_status =
            UsrMsgAskForProceed(
                    inp_buff,
                    inp_buff_size,
                    &proceed);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    if(!proceed)
        return OPTS_NEED_RETRY;

    SSTEraseChip();

    CommSendMsgFromFlash(
            usr_msg_job_done,
            (sizeof(usr_msg_job_done)-1),
            1);

    return OPTS_SUCCESS;
}

opts_status_t OptsCheckProdId(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size)
{
    if((inp_buff == NULL) || (inp_buff_size < 1))
        return OPTS_CRITICAL_ERR;

    CommSendMsgFromFlash(
            usr_msg_check_prod_id_info,
            (sizeof(usr_msg_check_prod_id_info)-1),
            1);

    uint8_t proceed;
    usr_msg_status_t usrmsg_status =
            UsrMsgAskForProceed(
                    inp_buff,
                    inp_buff_size,
                    &proceed);
    if(usrmsg_status == USR_MSG_FAILED)
    {
        return OPTS_CRITICAL_ERR;
    }
    else if(usrmsg_status == USR_MSG_INVALID_INPUT)
    {
        return OPTS_NEED_RETRY;
    }

    if(!proceed)
        return OPTS_NEED_RETRY;

    sst_prod_id_t prod_id = SSTReadProdId();

    usrmsg_status =
            UsrMsgDispProdIdAsAscii(
                    prod_id.man_id,
                    prod_id.dev_id,
                    out_buff,
                    out_buff_size);
    if(usrmsg_status == USR_MSG_FAILED)
        return OPTS_CRITICAL_ERR;

    usrmsg_status =
            UsrMsgProdIdCheck(
                    prod_id.man_id,
                    prod_id.dev_id);
    if(usrmsg_status == USR_MSG_INVALID_INPUT)
        return OPTS_NEED_RETRY;

    return OPTS_SUCCESS;
}
