#ifndef __BUZZER_H
#define __BUZZER_H

#include "typedefs.h"

void buzzer_init(void);

void buzzer_on(void);
void buzzer_off(void);

void buzzer_onAutoOff(u32 ms);

#endif
