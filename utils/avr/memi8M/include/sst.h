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
    SST_FAILED = 1
}sst_ec_t;

/*
 * @Initis pcb and puts sst ic into read mode.
 */
void SSTInit(void);

#endif /* INCLUDE_SST_H_ */
