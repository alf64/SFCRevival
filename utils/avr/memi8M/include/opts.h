/*
 * opts.h
 *
 *  Created on: 10 maj 2019
 *      Author: bmudlaff
 */

#ifndef INCLUDE_OPTS_H_
#define INCLUDE_OPTS_H_


typedef enum
{
    OPTS_SUCCESS = 0,
    OPTS_CRITICAL_ERR = 1,
    OPTS_NEED_RETRY = 2
}opts_status_t;

/*
 * @brief Implements read-bytes selection.
 *
 * @param inp_buff A pointer to buffer which will be used for obtaining user input via comm.
 * @param inp_buff_size A size (in bytes) of the given inp_buff.
 * This shall be no less than 8.
 * @param out_buff A pointer to buffer which will be used for giving output to user via comm.
 * @param out_buff_size A size (in bytes) of the given out_buff.
 * This shall be no less than 9.
 *
 * @returns opts_status_t
 * @retval OPTS_SUCCESS Means the function successfully ended its execution.
 * @retval OPTS_CRITICAL_ERR Means the function occurred critical error.
 * @retval OPTS_NEED_RETRY Means there is a need to restart (re-call) this function.
 */
opts_status_t OptsReadBytes(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size);

/*
 * @brief Implements write-bytes selection.
 *
 * @param inp_buff A pointer to buffer which will be used for obtaining user input via comm.
 * @param inp_buff_size A size (in bytes) of the given inp_buff.
 * This shall be no less than 8.
 * @param out_buff A pointer to buffer which will be used for giving output to user via comm.
 * @param out_buff_size A size (in bytes) of the given out_buff.
 * This shall be no less than 9.
 *
 * @returns opts_status_t
 * @retval OPTS_SUCCESS Means the function successfully ended its execution.
 * @retval OPTS_CRITICAL_ERR Means the function occurred critical error.
 * @retval OPTS_NEED_RETRY Means there is a need to restart (re-call) this function.
 */
opts_status_t OptsWriteBytes(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size);

/*
 * @brief Implements read-all selection.
 *
 * @param inp_buff A pointer to buffer which will be used for obtaining user input via comm.
 * @param inp_buff_size A size (in bytes) of the given inp_buff.
 * This shall be no less than 1.
 * @param out_buff A pointer to buffer which will be used for giving output to user via comm.
 * @param out_buff_size A size (in bytes) of the given out_buff.
 * This shall be no less than 9.
 *
 * @returns opts_status_t
 * @retval OPTS_SUCCESS If succeeded to read the whole memory.
 * @retval OPTS_CRITICAL_ERR If failed to read the whole memory.
 * @retval OPTS_NEED_RETRY Means there is a need to restart (re-call) this function.
 */
opts_status_t OptsReadAll(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size);

/*
 * @brief Implements write-all selection.
 *
 * @param inp_buff A pointer to buffer which will be used for obtaining user input via comm.
 * @param inp_buff_size A size (in bytes) of the given inp_buff.
 * This shall be no less than 1.
 * @param out_buff A pointer to buffer which will be used for giving output to user via comm.
 * @param out_buff_size A size (in bytes) of the given out_buff.
 * This shall be no less than 9.
 *
 * @returns opts_status_t
 * @retval OPTS_SUCCESS If succeeded to write the whole memory.
 * @retval OPTS_CRITICAL_ERR If failed to write the whole memory.
 * @retval OPTS_NEED_RETRY Means there is a need to restart (re-call) this function.
 */
opts_status_t OptsWriteAll(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size);

/*
 * @biref Implements erase-sector selection.
 *
 * @param inp_buff A pointer to buffer which will be used for obtaining user input via comm.
 * @param inp_buff_size A size (in bytes) of the given inp_buff.
 * This shall be no less than 8.
 * @param out_buff A pointer to buffer which will be used for giving output to user via comm.
 * @param out_buff_size A size (in bytes) of the given out_buff.
 * This shall be no less than 9.
 *
 * @returns opts_status_t
 * @retval OPTS_SUCCESS If succeeded to erase sector.
 * @retval OPTS_CRITICAL_ERR If failed to erase sector.
 * @retval OPTS_NEED_RETRY Means there is a need to restart (re-call) this function.
 */
opts_status_t OptsEraseSector(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size);

/*
 * @biref Implements erase-block selection.
 *
 * @param inp_buff A pointer to buffer which will be used for obtaining user input via comm.
 * @param inp_buff_size A size (in bytes) of the given inp_buff.
 * This shall be no less than 8.
 * @param out_buff A pointer to buffer which will be used for giving output to user via comm.
 * @param out_buff_size A size (in bytes) of the given out_buff.
 * This shall be no less than 9.
 *
 * @returns opts_status_t
 * @retval OPTS_SUCCESS If succeeded to erase block.
 * @retval OPTS_CRITICAL_ERR If failed to erase block.
 * @retval OPTS_NEED_RETRY Means there is a need to restart (re-call) this function.
 */
opts_status_t OptsEraseBlock(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size);

/*
 * @biref Implements erase-all selection.
 *
 * @param inp_buff A pointer to buffer which will be used for obtaining user input via comm.
 * @param inp_buff_size A size (in bytes) of the given inp_buff.
 * This shall be no less than 1.
 *
 * @returns opts_status_t
 * @retval OPTS_SUCCESS If succeeded to erase all memory.
 * @retval OPTS_CRITICAL_ERR If failed to erase all memory.
 * @retval OPTS_NEED_RETRY Means there is a need to restart (re-call) this function.
 */
opts_status_t OptsEraseAll(
        unsigned char* inp_buff,
        uint32_t inp_buff_size);

/*
 * @brief Implements check-product-id selection.
 *
 * @param inp_buff A pointer to buffer which will be used for obtaining user input via comm.
 * @param inp_buff_size A size (in bytes) of the given inp_buff.
 * This shall be no less than 1.
 * @param out_buff A pointer to buffer which will be used for giving output to user via comm.
 * @param out_buff_size A size (in bytes) of the given out_buff.
 * This shall be no less than 3.
 *
 * @returns opts_status_t
 * @retval OPTS_SUCCESS If succeeded to check product id.
 * @retval OPTS_CRITICAL_ERR If failed to check product id.
 * @retval OPTS_NEED_RETRY Means there is a need to restart (re-call) this function.
 */
opts_status_t OptsCheckProdId(
        unsigned char* inp_buff,
        uint32_t inp_buff_size,
        unsigned char* out_buff,
        uint32_t out_buff_size);

#endif /* INCLUDE_OPTS_H_ */
