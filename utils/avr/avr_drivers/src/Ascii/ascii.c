/*
 * ascii.c
 *
 *  Created on: 6 maj 2019
 *      Author: bmudlaff
 */

#include <stdio.h>
#include <avr/io.h>
#include <Ascii/ascii.h>

uint8_t is_hex(
        const unsigned char* a,
        uint8_t size)
{
    for(uint8_t i = 0; i < size; i++)
    {
        if((a[i] >= 'A' && a[i] <= 'F') ||
                (a[i] >= 'a' && a[i] <= 'f') ||
                (a[i] >= '0' && a[i] <= '9'))
        {
            // it's okay, go on with the loop
        }
        else
        {
            return 0; // false, some char is out-of-hex range
        }
    }

    return 1; // true
}

uint8_t hex_to_bt(const unsigned char* hx)
{
    if(hx == NULL)
        return 0;

    uint8_t ret = 0;
    uint8_t offset = 0;
    uint8_t addon = 0;

    for(uint8_t i = 0; i < 2; i++)
    {
        if((hx[i] >= 'A' && hx[i] <= 'F'))
        {
            offset = 'A';
            addon = 10;
        }
        else if((hx[i] >= 'a' && hx[i] <= 'f'))
        {
            offset = 'a';
            addon = 10;
        }
        else if((hx[i] >= '0' && hx[i] <= '9'))
        {
            offset = '0';
            addon = 0;
        }
        else
        {
            /*
             * given hx is not in hex range - shall not fall here
             */
            return 0;
        }

        ret += ((hx[i] - offset)+addon)<<((1-i)*4);
    }

    return ret;
}

void bt_to_hex(
        uint8_t bt,
        unsigned char* hx,
        uint8_t hx_size)
{
    if(hx == NULL)
        return;
    if(hx_size < 2)
        return;

    uint8_t part = 0;
    uint8_t offset = 0;
    uint8_t offdiff = 0;

    for(uint8_t i = 0; i < 2; i++)
    {
        part = ((bt >> (1-i)*4) & 0xF);
        if((part >= 0xA) || (part <= 0xF))
        {
            offset = 'A';
            offdiff = part - 0xA;
        }
        else
        {
            offset = '0';
            offdiff = part - 0;
        }

        hx[i] = offset + offdiff;
    }

    return;
}

ascii_status_t AsciiToU32(
        const unsigned char* input,
        uint32_t* output)
{
    if(input == NULL || output == NULL)
        return ASCII_FAILED;

    const unsigned char* cursor = input;
    ascii_status_t status;
    uint32_t ret = 0;

    if(is_hex(input, 8))
    {
        for(uint8_t i = 0; i < 4; i++)
        {
            uint32_t tmp = (uint32_t)hex_to_bt(cursor);
            ret += (tmp)<<((3-i)*8);
            cursor += 2;
        }

        status = ASCII_SUCCESS;
    }
    else
    {
        status = ASCII_INVALID_RANGE;
    }

    *output = ret;
    return status;
}

ascii_status_t U32ToAscii(
        uint32_t input,
        unsigned char* output,
        uint8_t output_size)
{
    if(output == NULL)
        return ASCII_FAILED;
    if(output_size < 9)
        return ASCII_FAILED;

    ascii_status_t status = ASCII_SUCCESS;
    uint8_t bt = 0;

    for(uint8_t i = 0; i < 4; i++)
    {
        bt = (uint8_t)((input>>((3-i)*8))&0xFF);
        bt_to_hex(bt, &output[(i*2)], 2);
    }
    // append NULL at the end of output
    output[8] = '\0';

    return status;
}

