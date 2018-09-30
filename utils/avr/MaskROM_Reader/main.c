/*
 * main.c
 *
 *  Created on: 12 lut 2018
 *      Author: alf64
 */

/*
 * This program reads Ninny's MaskROM 8Mbit.
 *
 * HW requirements: MaskROM_Reader.
 *
 * F_CPU shall be set in Eclipse or manually to 11,0592 MHz.
 * The TARGET shall be set to Atmega32.
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "LEDdrv/led.h"
#include "Reader/reader.h"
#include "Comm/comm.h"
#include "USARTdrv/usart.h"


// ------ user interaction ------
const char msg_01[] PROGMEM = "Welcome to MaskROM_Reader. What do you want to do ?";
const char msg_02[] PROGMEM = "1. Read a few selected bytes from MaskROM.";
const char msg_03[] PROGMEM = "2. Read 1 MegaByte from MaskROM.";
const char msg_04[] PROGMEM = "Choose [1 - 2]: ";
const char msg_05[] PROGMEM = "Unrecognized option.";
char user_input[1];
// ------------------------------

int main(void)
{
    // turn ON interrupts
    sei();

    // init led driver
    LEDS_INIT;
    LEDS_OFF;

    // init communication (via UART) mechanism
    comm_status_t c_stat = CommInit(
            57600,
            F_CPU,
            NULL,
            0);
    if(c_stat != COMM_SUCCESS)
        while(1); // stuck here forever

    // first question to user...
    CommSendMsgFromFlash(msg_01, (sizeof(msg_01)-1));
    CommSendMsgFromFlash(msg_02, (sizeof(msg_02)-1));
    CommSendMsgFromFlash(msg_03, (sizeof(msg_03)-1));
    CommSendMsgFromFlash(msg_04, (sizeof(msg_04)-1));
    while(CommGetMsg(1, user_input) != COMM_SUCCESS); // w8 for user input
    CommSendMsg(user_input, 1); // echo
    //user_input[0] = '2';

    InitReader();
    reader_status_t r_stat = ResetReader();
    if(r_stat != READER_SUCCESS)
        while(1); // stuck forever

    switch(user_input[0])
    {
        case '1':
        {
            for(uint32_t i = 0; i < 1; i++)
            {
                uint8_t bt = ReadByteReader((uint32_t)0x5B02);
                c_stat = CommSendByteAsHexAscii(bt);
                _delay_us(1.0f);
                bt = ReadByteReader((uint32_t)0x6FFF);
                c_stat = CommSendByteAsHexAscii(bt);
                _delay_us(1.0f);
                bt = ReadByteReader((uint32_t)0x7B6C);
                c_stat = CommSendByteAsHexAscii(bt);
                _delay_us(1.0f);
                bt = ReadByteReader((uint32_t)0xCA4D);
                c_stat = CommSendByteAsHexAscii(bt);
                _delay_us(1.0f);
                bt = ReadByteReader((uint32_t)0x82BEF);
                c_stat = CommSendByteAsHexAscii(bt);
                _delay_us(1.0f);
                bt = ReadByteReader((uint32_t)0x82BF5);
                c_stat = CommSendByteAsHexAscii(bt);
                _delay_us(1.0f);
                bt = ReadByteReader((uint32_t)0x87B35);
                c_stat = CommSendByteAsHexAscii(bt);
            }
            break;
        }
        case '2':
        {
            for(uint32_t i = 0; i < 1048576; i++)
            {
                uint8_t bt = ReadByteReader(i);
                UsartTransmit(bt);
            }
            break;
        }
        default:
        {
            CommSendMsgFromFlash(msg_05, (sizeof(msg_05)-1));
            break;
        }
    }

    LED_ON(LED1_PIN);

    for(;;); // forever loop

    return 0;
}

