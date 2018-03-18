/*
 * led.h
 *
 *  Created on: 3 mar 2018
 *      Author: alf64
 */

#ifndef LEDDRV_LED_H_
#define LEDDRV_LED_H_

#define LED1_PIN PC0
#define LED2_PIN PC1

#define LEDS_PORT PORTC
#define LEDS_DDR DDRC

#define LEDS_INIT LEDS_DDR |= (1<<LED1_PIN)|(1<<LED2_PIN)
#define LEDS_ON LEDS_PORT &= ~((1<<LED1_PIN)|(1<<LED2_PIN))
#define LEDS_OFF LEDS_PORT |= (1<<LED1_PIN)|(1<<LED2_PIN)

#define LED_ON(x) (LEDS_PORT &= ~(1<<x))
#define LED_OFF(x) (LEDS_PORT |= (1<<x))

#endif /* LEDDRV_LED_H_ */
