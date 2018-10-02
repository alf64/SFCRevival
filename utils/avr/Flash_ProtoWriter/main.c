/*
 * main.c
 *
 *  Created on: 30 wrz 2018
 *      Author: alf64
 */

/*
 * This program writes FLASH memory with values provided via UART.
 *
 * HW requirements: MaskRom_Emu.
 *
 * F_CPU shall be set in Eclipse or manually to 11,0592 MHz.
 * The TARGET shall be set to Atmega32.
 */

/*
 * Supported FLASH ICs.
 */
#define SST39VF1681_1681 0xFF
#define SST39VF1681_1682 0xFE

/*
 * Selected FLASH IC for this utility to operate on.
 */
#define FLASH_IC SST39VF1681_1681

/*
 * Supported FLASH check.
 */
#ifndef FLASH_IC
#error "FLASH_IC not defined."
#endif
#if FLASH_IC == SST39VF1681_1681
#elif FLASH_IC == SST39VF1681_1682
#else
#error "Unsupported FLASH_IC given."
#endif

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "LEDdrv/led.h"
#include "Comm/comm.h"
#include "USARTdrv/usart.h"
#include "FlashRW/flashrw.h"


// ------ user interaction ------
const char msg_01[] PROGMEM = "Welcome to FLASH_ProtoWriter. What do you want to do ?";
const char msg_02[] PROGMEM = "1. Read the first byte from FLASH IC.";
const char msg_03[] PROGMEM = "1. Write the first byte to FLASH IC.";
const char msg_04[] PROGMEM = "Choose [1 - 2]: ";
const char msg_05[] PROGMEM = "Unrecognized option.";
char user_input[1];
// ------------------------------

int main(void)
{
	// turn ON interrupts
	sei();

	/*
	 * do not init LEDs - led driver pins conflict with flashrw pins
	 */

    // init communication (via UART) mechanism
    comm_status_t c_stat = CommInit(
            57600,
            F_CPU,
            NULL,
            0);
    if(c_stat != COMM_SUCCESS)
        while(1); // stuck here forever

    // first interaction with user...
    CommSendMsgFromFlash(msg_01, (sizeof(msg_01)-1));
    CommSendMsgFromFlash(msg_02, (sizeof(msg_02)-1));
    CommSendMsgFromFlash(msg_03, (sizeof(msg_03)-1));
    CommSendMsgFromFlash(msg_04, (sizeof(msg_04)-1));
    while(CommGetMsg(1, user_input) != COMM_SUCCESS); // w8 for user input
    CommSendMsg(user_input, 1); // echo

    InitFlashRW();
    flashrw_status_t f_stat = ResetFlashRW();
    if(f_stat != FLASHRW_SUCCESS)
        while(1); // stuck forever

    switch(user_input[0])
    {
        case '1':
        {
            //TODO: implement single byte read here
            break;
        }
        case '2':
        {
            //TODO: implement single byte write here
            break;
        }
        default:
        {
            CommSendMsgFromFlash(msg_05, (sizeof(msg_05)-1));
            break;
        }
    }

	for(;;); //forever loop

	return 0;
}
