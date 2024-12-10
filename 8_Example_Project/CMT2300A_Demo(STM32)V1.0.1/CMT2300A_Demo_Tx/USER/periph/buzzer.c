#include "buzzer.h"
#include "common.h"
#include "system.h"
#include "gpio_defs.h"

void buzzer_init(void)
{
    SET_GPIO_L(BUZZER_GPIO);
    SET_GPIO_OUT(BUZZER_GPIO);
    SET_GPIO_L(BUZZER_GPIO);
}

void buzzer_on(void)
{
    SET_GPIO_H(BUZZER_GPIO);
}

void buzzer_off(void)
{
    SET_GPIO_L(BUZZER_GPIO);
}

void buzzer_onAutoOff_proc(void)
{
    buzzer_off();
}

void buzzer_onAutoOff(u32 ms)
{
    time_server_setDelayRunTask(DELAY_RUN_TASK_BUZZER, buzzer_onAutoOff_proc, ms);
    buzzer_on();
}
