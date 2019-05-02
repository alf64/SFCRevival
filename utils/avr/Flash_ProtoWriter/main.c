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
#include "hal/usart.h"
#include "FlashRW/flashrw.h"


// ------ user interaction ------
const char msg_01[] PROGMEM = "\nWelcome to FLASH_ProtoWriter. What do you want to do ?";
const char msg_02[] PROGMEM = "1. Erase the first sector from FLASH IC.";
const char msg_03[] PROGMEM = "2. Read the first byte from FLASH IC.";
const char msg_04[] PROGMEM = "3. Write the first byte to FLASH IC.";
const char msg_05[] PROGMEM = "4. Erase the whole FLASH chip.";
const char msg_06[] PROGMEM = "5. Debug scenario 1.";

const char msg_10[] PROGMEM = "Choose [1 - 3]: ";
const char msg_11[] PROGMEM = "Unrecognized option.";
const char msg_12[] PROGMEM = "First sector erased.";
const char msg_13[] PROGMEM = "First byte programmed.";
const char msg_14[] PROGMEM = "Flash chip erased.";
unsigned char user_input[1];
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

    InitFlashRW();
    flashrw_status_t f_stat = ResetFlashRW();
    if(f_stat != FLASHRW_SUCCESS)
        while(1); // stuck forever

    // warm-up time
    _delay_us(100.0f);

    for(;;)
    {
        // interaction with user...
        CommSendMsgFromFlash(msg_01, (sizeof(msg_01)-1));
        CommSendMsgFromFlash(msg_02, (sizeof(msg_02)-1));
        CommSendMsgFromFlash(msg_03, (sizeof(msg_03)-1));
        CommSendMsgFromFlash(msg_04, (sizeof(msg_04)-1));
        CommSendMsgFromFlash(msg_05, (sizeof(msg_05)-1));
        CommSendMsgFromFlash(msg_06, (sizeof(msg_06)-1));
        CommSendMsgFromFlash(msg_10, (sizeof(msg_10)-1));
        while(CommGetMsg(1, user_input, sizeof(user_input)) != COMM_SUCCESS); // w8 for user input
        CommSendMsg(user_input, 1); // echo

        switch(user_input[0])
        {
            case '1':
            {
                SectorEraseFlashRW((uint32_t)0x0);
                CommSendMsgFromFlash(msg_12, (sizeof(msg_12)-1));
                break;
            }
            case '2':
            {
                uint8_t bt = ReadByteFlashRW((uint32_t)0x00000000);
                c_stat = CommSendByteAsHexAscii(bt);
                break;
            }
            case '3':
            {
                WriteByteFlashRW(0xAB, (uint32_t)0x00000000);
                CommSendMsgFromFlash(msg_13, (sizeof(msg_13)-1));
                break;
            }
            case '4':
            {
                ChipEraseFlashRW();
                CommSendMsgFromFlash(msg_14, (sizeof(msg_14)-1));
                break;
            }
            case '5':
            {
                uint8_t manufacturer_id = DebugScenario01FlashRW();
                c_stat = CommSendByteAsHexAscii(manufacturer_id);
                break;
            }
            default:
            {
                CommSendMsgFromFlash(msg_11, (sizeof(msg_11)-1));
                break;
            }
        }
    }

    for(;;); //forever loop

    return 0;
}
