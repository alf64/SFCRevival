/*
 * main.c
 *
 *  Created on: 12 lut 2018
 *      Author: alf64
 */

/*
 * This program reads Ninny's MaskROM 8Mbit.
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


// ------ user interaction ------
const char msg_01[] PROGMEM = "Welcome to MaskROM_Reader. What do you want to do ?";
const char msg_02[] PROGMEM = "1. Read first bank from MaskROM.";
const char msg_03[] PROGMEM = "Choose [1 - 1]: ";
const char msg_04[] PROGMEM = "Unrecognized option.";
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
            19200,
            F_CPU,
            NULL,
            0);
    if(c_stat != COMM_SUCCESS)
        while(1); // stuck here forever

    // first question to user...
    CommSendMsgFromFlash(msg_01, (sizeof(msg_01)-1));
    CommSendMsgFromFlash(msg_02, (sizeof(msg_02)-1));
    CommSendMsgFromFlash(msg_03, (sizeof(msg_03)-1));
    while(CommGetMsg(1, user_input) != COMM_SUCCESS); // w8 for user input
    CommSendMsg(user_input, 1); // echo

    InitReader();
    reader_status_t r_stat = ResetReader();
    if(r_stat != READER_SUCCESS)
        while(1); // stuck forever

    switch(user_input[0])
    {
        case '1':
        {
            for(uint32_t i = 0; i < 65536; i++)
            {
                uint8_t bt = ReadByteReader(i);
                c_stat = CommSendByteAsHexAscii(bt);
                _delay_us(1);
            }
            break;
        }
        default:
        {
            CommSendMsgFromFlash(msg_04, (sizeof(msg_04)-1));
            break;
        }
    }

    LED_ON(LED1_PIN);

    for(;;); // forever loop

    return 0;
}

