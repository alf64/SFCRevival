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
 * The target shall be set to Atmega32.
 */

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "LEDdrv/led.h"
#include "Reader/reader.h"

int main(void)
{
    LEDS_INIT;
    LEDS_OFF;

    InitReader();

    LED_ON(LED1_PIN);

    for(;;)
    {
        // do something here...
    }

    return 0;
}
