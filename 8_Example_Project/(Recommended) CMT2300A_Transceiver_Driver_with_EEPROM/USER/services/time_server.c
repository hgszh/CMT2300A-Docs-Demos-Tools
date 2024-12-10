#include <string.h>
#include "time_server.h"
#include "system.h"
#include "gpio_defs.h"
#include "stm32f10x_conf.h"

volatile delay_run_task g_delayRunTasks[DELAY_RUN_TASK_SIZE];
volatile u32 g_nSysTickCount = 0;

volatile u32 g_nUserTimeout = 0;
volatile u32 g_nUserTimeCnt = 0;

p_time_server_proc_run g_time_server_proc_run5;
p_time_server_proc_run g_time_server_proc_run6;

void time_server_init(void)
{
    g_nUserTimeout          = 0;
    g_time_server_proc_run5 = 0;
    g_time_server_proc_run6 = 0;

    memset((void*)g_delayRunTasks, 0, sizeof(g_delayRunTasks));
}

void time_server_setDelayRunTask(u8 index, p_time_server_proc_run func, u32 ms)
{
    u8 i, count;

    for(i=0, count=0; i<DELAY_RUN_TASK_SIZE; i++)
    {
        if( g_delayRunTasks[i].func && 
            g_delayRunTasks[i].ms_count )
        {
            count++;
        }
    }

    g_delayRunTasks[index].func     = func;
    g_delayRunTasks[index].ms_count = ms;

    if(count==0) {
        TIM_Cmd(TIM5, ENABLE);
    }
}

void time_server_interrupt_proc5(void)
{
    u8 i, count;

    for(i=0, count=0; i<DELAY_RUN_TASK_SIZE; i++)
    {
        if( g_delayRunTasks[i].func && 
            g_delayRunTasks[i].ms_count )
        {
            g_delayRunTasks[i].ms_count--;
            if(g_delayRunTasks[i].ms_count==0) {
                g_delayRunTasks[i].func();
            }

            count++;
        }
    }

    if(count==0) {
        TIM_Cmd(TIM5, DISABLE);
    }
}

void SetUserTimeout(u32 nTimeout)
{
    g_nUserTimeout = nTimeout;
    get_system_timestamp(&g_nUserTimeCnt);
}

U8 IsUserTimeout(void)
{
    return (get_system_relative_ms_count(&g_nUserTimeCnt) >g_nUserTimeout) ?1 :0;
}

void system_delay_ms(u32 nms)
{
    u32 nDelayUsCnt = nms*10;
    
    while(nDelayUsCnt--)
        system_delay_100us(1);
}

void system_delay_10us(u32 n)
{
    u32 nDelayUsCnt = n*10;
    
    while(nDelayUsCnt--) {
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); 
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }
}

void system_delay_100us(u32 n)
{
    u32 nDelayUsCnt = n*100;
    
    while(nDelayUsCnt--) {
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); 
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }
}

void system_delay_us(u32 n)
{
    while(n--) 
    {
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); 
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }
}

void system_delay_500ns(void)
{
    //while(N500ns--)
    //{
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP();
    //}
}
