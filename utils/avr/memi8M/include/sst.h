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

typedef struct
{
    uint8_t man_id;
    uint8_t dev_id;
}sst_prod_id_t;

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
 * @attention
 * Max addr is: 24 bit (assuming only 3 x 8-bit shift registers are present).
 *
 * @returns sst_ec_t
 * @retval SST_SUCCESS Succeeded to read the data.
 * @retval SST_FAILED Critical error occured (data not read).
 */
sst_ec_t SSTRead(
        uint32_t addr,
        uint8_t* retbt);

/*
 * @brief Writes byte of data to the given addr.
 *
 * @param addr Address to which the data will be written.
 * @param writebt Data to be written.
 *
 * @attention
 * Max addr is: 24 bit (assuming only 3 x 8-bit shift registers are present).
 *
 * @returns sst_ec_t
 * @retval SST_SUCCESS Succeeded to read the data.
 * @retval SST_FAILED Critical error occured (data not read).
 */
sst_ec_t SSTWrite(
        uint32_t addr,
        uint8_t writebt);

/*
 * @brief Erases sector of memory.
 *
 * @param sector_addr An address of the sector to be erased.
 * This shall not be higher than BOARD_MAX_SECTOR_ADDR.
 *
 * @retval sst_ec_t
 * @retval SST_SUCCESS Succeeded to erase the sector.
 * @retval SST_INVALID_INPUT Means invalid sector_addr was given.
 */
sst_ec_t SSTEraseSector(uint32_t sector_addr);

/*
 * @brief Erases block of memory.
 *
 * @param block_addr An address of the block to be erased.
 * This shall not be higher than BOARD_MAX_BLOCK_ADDR.
 *
 * @retval sst_ec_t
 * @retval SST_SUCCESS Succeeded to erase the block.
 * @retval SST_INVALID_INPUT Means invalid block_addr was given.
 */
sst_ec_t SSTEraseBlock(uint32_t block_addr);

/*
 * @brief Erases chip.
 */
void SSTEraseChip(void);

/*
 * @brief Reads SST product id information.
 *
 * @returns sst_prod_id_t Product id structure with read information fields.
 */
sst_prod_id_t SSTReadProdId(void);

#endif /* INCLUDE_SST_H_ */
