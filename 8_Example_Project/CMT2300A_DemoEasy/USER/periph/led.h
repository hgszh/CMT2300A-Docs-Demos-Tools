#ifndef __LED_H
#define __LED_H

#include "typedefs.h"

#define LED_ALL			0
#define LED_INDEX1		1
#define LED_INDEX2		2
#define LED_INDEX3		3

void led_init(void);
void led_on(u8 index);
void led_off(u8 index);
void led_reverse(u8 index);
u8 led_is_on(u8 index);

void led_onAutoOff(u8 index, u32 ms);


#endif
