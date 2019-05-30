/*
 * sst.h
 *
 *  Created on: 7 maj 2019
 *      Author: bmudlaff
 */

#ifndef INCLUDE_SST_H_
#define INCLUDE_SST_H_

/*
 * Read, write, erase functionality here.
 */

typedef enum
{
    SST_SUCCESS = 0,
    SST_FAILED = 1,
    SST_INVALID_INPUT = 2
}sst_ec_t;

/*
 * @brief Returns pcb to initial state (sets default pin values).
 */
void SSTRet(void);

/*
 * @Initis pcb and sets default pin values.
 */
void SSTInit(void);

/*
 * @brief Reads byte of data from the given addr.
 *
 * @param addr Address from which the data shall be read.
 * @param retbt A pointer where the read data shall be stored.
 *
 * @returns sst_ec_t
 * @retval SST_SUCCESS Succeeded to read the data.
 * @retval SST_FAILED Critical error occured (data not read).
 * @retval SST_INVALID_INPUT Given addr is outside of acceptable range.
 */
sst_ec_t SSTRead(
        uint32_t addr,
        uint8_t* retbt);

#endif /* INCLUDE_SST_H_ */
