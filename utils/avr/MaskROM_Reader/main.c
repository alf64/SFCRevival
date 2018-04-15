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
const char msg_01[] PROGMEM = "Do you want to init the Reader ? (y/n)";
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
    while(CommGetMsg(1, user_input) != COMM_SUCCESS); // w8 for user input
    CommSendMsg(user_input, 1); // echo

    /*InitReader();
    reader_status_t r_stat = ResetReader();
    if(r_stat != READER_SUCCESS)
        while(1);*/ // stuck forever

    LED_ON(LED1_PIN);

    for(;;)
    {
        // do something here...
    }

    return 0;
}

