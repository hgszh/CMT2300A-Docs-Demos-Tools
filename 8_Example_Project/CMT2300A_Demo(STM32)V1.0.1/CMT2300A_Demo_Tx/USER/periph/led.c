#include "led.h"
#include "gpio_defs.h"
#include "common.h"
#include "time_server.h"
#include "stm32f10x_conf.h"

void led_init(void)
{
    SET_GPIO_L(LED1_GPIO);
    SET_GPIO_L(LED2_GPIO);
    SET_GPIO_L(LED3_GPIO);

    SET_GPIO_OUT(LED1_GPIO);
    SET_GPIO_OUT(LED2_GPIO);
    SET_GPIO_OUT(LED3_GPIO);
}

void led_on(u8 index)
{
    switch(index)
    {
    case LED_INDEX1:    SET_GPIO_H(LED1_GPIO);    break;
    case LED_INDEX2:    SET_GPIO_H(LED2_GPIO);    break;
    case LED_INDEX3:    SET_GPIO_H(LED3_GPIO);    break;

    case LED_ALL:
        SET_GPIO_H(LED1_GPIO);
        SET_GPIO_H(LED2_GPIO);
        SET_GPIO_H(LED3_GPIO);
        break;
    }
}

void led_off(u8 index)
{
    switch(index)
    {
    case LED_INDEX1:    SET_GPIO_L(LED1_GPIO);    break;
    case LED_INDEX2:    SET_GPIO_L(LED2_GPIO);    break;
    case LED_INDEX3:    SET_GPIO_L(LED3_GPIO);    break;

    case LED_ALL:
        SET_GPIO_L(LED1_GPIO);
        SET_GPIO_L(LED2_GPIO);
        SET_GPIO_L(LED3_GPIO);
        break;
    }
}

void led_reverse(u8 index)
{
    if(index==LED_ALL || index==LED_INDEX1)
    {
        if(READ_GPIO_PIN(LED1_GPIO))
            SET_GPIO_L(LED1_GPIO);
        else
            SET_GPIO_H(LED1_GPIO);
    }

    if(index==LED_ALL || index==LED_INDEX2)
    {
        if(READ_GPIO_PIN(LED2_GPIO))
            SET_GPIO_L(LED2_GPIO);
        else
            SET_GPIO_H(LED2_GPIO);
    }

    if(index==LED_ALL || index==LED_INDEX3)
    {
        if(READ_GPIO_PIN(LED3_GPIO))
            SET_GPIO_L(LED3_GPIO);
        else
            SET_GPIO_H(LED3_GPIO);
    }
}

u8 led_is_on(u8 index)
{
    u8 bRet = 0;

    switch(index)
    {
    case LED_INDEX1:    bRet = READ_GPIO_PIN(LED1_GPIO);    break;
    case LED_INDEX2:    bRet = READ_GPIO_PIN(LED2_GPIO);    break;
    case LED_INDEX3:    bRet = READ_GPIO_PIN(LED3_GPIO);    break;

    case LED_ALL:
        bRet |= READ_GPIO_PIN(LED1_GPIO);
        bRet |= READ_GPIO_PIN(LED2_GPIO);
        bRet |= READ_GPIO_PIN(LED3_GPIO);
        break;
    }

    return bRet>0 ?1 :0;
}

void led1_onAutoOff_proc(void)
{
    led_off(LED_INDEX1);
}

void led2_onAutoOff_proc(void)
{
    led_off(LED_INDEX2);
}

void led3_onAutoOff_proc(void)
{
    led_off(LED_INDEX3);
}

void led_onAutoOff(u8 index, u32 ms)
{
    switch(index)
    {
    case LED_INDEX1:    time_server_setDelayRunTask(DELAY_RUN_TASK_LED1, led1_onAutoOff_proc, ms);    break;    
    case LED_INDEX2:    time_server_setDelayRunTask(DELAY_RUN_TASK_LED2, led2_onAutoOff_proc, ms);    break;
    case LED_INDEX3:    time_server_setDelayRunTask(DELAY_RUN_TASK_LED3, led3_onAutoOff_proc, ms);    break;
    }

    led_on(index);
}
