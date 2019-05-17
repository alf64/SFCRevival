/*
 * ascii.h
 *
 *  Created on: 6 maj 2019
 *      Author: bmudlaff
 */

#ifndef INCLUDE_ASCII_ASCII_H_
#define INCLUDE_ASCII_ASCII_H_

typedef enum _ascii_status_t
{
    ASCII_SUCCESS = 0,
    ASCII_INVALID_RANGE = 1,
    ASCII_FAILED = 2
}ascii_status_t;

/*
 * @brief Function converts char (ascii) to uint8_t.
 *
 * @details
 * This function can be easily used to convert hex
 * values to bytes.
 *
 * @attention
 * This function assumes:
 * - given ascii char is 2-bytes long
 * - given ascii char values are from ranges: '0' - '9', 'A'-'F', 'a' - 'f'
 * - big endian byte order (the very first byte is the most significant)
 *
 * @param input A pointer to ascii characters of size: 2 bytes
 * @param output A pointer where the result of this function shall be stored.
 *
 * @returns ascii_status_t
 */
ascii_status_t AsciiToU8(
        const unsigned char* input,
        uint8_t* output);

/*
 * @brief Function converts uint8_t to char (ascii).
 *
 * @param input An uint8_t to be converted.
 * @param output A char array where the conversion result shall be stored.
 * @param output_size A size (in bytes) of the given output array.
 * output_size shall be at least of size: 3 bytes (2 for actual data, 1 for '\0' appending purposes).
 *
 * @attention
 * This function appends NULL ('\0;) at the end of the given output array.
 *
 * @returns ascii_status_t
 * @retval ASCII_SUCCESS If succeeded to perform conversion.
 * @retval ASCII_FAILED If failed to perform conversion.
 */
ascii_status_t U8ToAscii(
        uint8_t input,
        unsigned char* output,
        uint8_t output_size);

/*
 * @brief Function converts char (ascii) to uint32_t.
 *
 * @details
 * This function can be easily used to convert hex
 * values to bytes.
 *
 * @attention
 * This function assumes:
 * - given ascii char is 8-bytes long
 * - given ascii char values are from ranges: '0' - '9', 'A'-'F', 'a' - 'f'
 * - big endian byte order (the very first byte is the most significant)
 *
 * @param input A pointer to ascii characters of size: 8 bytes
 * @param output A pointer where the result of this function shall be stored.
 *
 * @returns ascii_status_t
 */
ascii_status_t AsciiToU32(
        const unsigned char* input,
        uint32_t* output);

/*
 * @brief Function converts uint32_t to char (ascii).
 *
 * @param input An uint32_t to be converted.
 * @param output A char array where the conversion result shall be stored.
 * @param output_size A size (in bytes) of the given output array.
 * output_size shall be at least of size: 9 bytes (8 for actual data, 1 for '\0' appending purposes).
 *
 * @attention
 * This function appends NULL ('\0;) at the end of the given output array.
 *
 * @returns ascii_status_t
 * @retval ASCII_SUCCESS If succeeded to perform conversion.
 * @retval ASCII_FAILED If failed to perform conversion.
 */
ascii_status_t U32ToAscii(
        uint32_t input,
        unsigned char* output,
        uint8_t output_size);

#endif /* INCLUDE_ASCII_ASCII_H_ */
