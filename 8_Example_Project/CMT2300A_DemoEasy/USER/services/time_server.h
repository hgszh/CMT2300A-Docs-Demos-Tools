#ifndef __TIME_SERVER_H
#define __TIME_SERVER_H

#include "typedefs.h"

typedef u32 system_timestamp;
typedef void (*p_time_server_proc_run)(void);

typedef struct __delay_run_task {
    u32 ms_count;
    p_time_server_proc_run func;    
} delay_run_task;

#define DELAY_RUN_TASK_SIZE        16
#define DELAY_RUN_TASK_LED1        0
#define DELAY_RUN_TASK_LED2        1
#define DELAY_RUN_TASK_LED3        2
#define DELAY_RUN_TASK_BUZZER      3
#define DELAY_RUN_TASK_TEST        4
#define DELAY_RUN_TASK_TEST2       5

extern volatile delay_run_task g_delayRunTasks[DELAY_RUN_TASK_SIZE];

extern volatile u32 g_nUserTimeout;
extern volatile u32 g_nSysTickCount;

extern p_time_server_proc_run g_time_server_proc_run5;
extern p_time_server_proc_run g_time_server_proc_run6;

#define get_system_timestamp(x)             ( (*x) = g_nSysTickCount )
#define get_system_relative_ms_count(x)     ( g_nSysTickCount - (*x) )
#define get_system_relative_sec_count(x)    ( get_system_relative_us_count(x)/1000u )

void time_server_init(void);
void time_server_setDelayRunTask(u8 index, p_time_server_proc_run func, u32 ms);
void time_server_interrupt_proc5(void);

void SetUserTimeout(u32 nTimeout);
u8 IsUserTimeout(void);

void system_delay_ms(u32 n);
void system_delay_10us(u32 n);
void system_delay_100us(u32 n);
void system_delay_us(u32 n);
void system_delay_500ns(void);


#endif
