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
 * @brief This function implements read-bytes selection.
 *
 * @param inp_buff A pointer to buffer which will be used for obtaining user input via comm.
 * @param inp_buff_size A size (in bytes) of the given inp_buff.
 * @param out_buff A pointer to buffer which will be used for giving output to user via comm.
 * @param out_buff_size A size (in bytes) of the given out_buff.
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

#endif /* INCLUDE_OPTS_H_ */
