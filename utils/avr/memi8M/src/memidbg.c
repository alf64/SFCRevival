/*
 * memidbg.c
 *
 *  Created on: 4 gru 2019
 *      Author: alf64
 */

#include <stdio.h>
#include <avr/io.h>
#include "pcbhal.h"
#include "memidbg.h"

void MemiDbgWaitForBtnPush(void)
{
    // wait until the button is released (if someone is still holding it)
    while(pcbhal_dbg_btn_get() == DBG_BTN_PRESSED)
    {
        // debounce
        while(pcbhal_dbg_btn_get() == DBG_BTN_PRESSED){};
        _delay_ms(100);
        while(pcbhal_dbg_btn_get() == DBG_BTN_PRESSED){};
        _delay_ms(1);
        while(pcbhal_dbg_btn_get() == DBG_BTN_PRESSED){};
        _delay_ms(1);
        while(pcbhal_dbg_btn_get() == DBG_BTN_PRESSED){};
        _delay_ms(1);
    }

    // wait for button tu be pushed
    while(pcbhal_dbg_btn_get() == DBG_BTN_RELEASED)
    {
        // debounce
        while(pcbhal_dbg_btn_get() == DBG_BTN_RELEASED);
        _delay_ms(100);
        while(pcbhal_dbg_btn_get() == DBG_BTN_RELEASED);
        _delay_ms(1);
        while(pcbhal_dbg_btn_get() == DBG_BTN_RELEASED);
        _delay_ms(1);
        while(pcbhal_dbg_btn_get() == DBG_BTN_RELEASED);
        _delay_ms(1);
    }
}



