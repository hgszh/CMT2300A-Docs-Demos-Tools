/********************  (C) COPYRIGHT 2012  Cmostek Microelectronics Co., Ltd.  ********************
* File Name          : stm32f10x_it.c
* Author             : IC Application Team
* Version            : V1.1.0
* Date               : 2010/5/29
* Description        : System interrupt control program
**************************************************************************************************/


#include "stm32f10x_conf.h"

#include "system.h"
#include "time_server.h"
#include "interrupt_server.h"

void EXTI0_IRQHandler(void)
{
	if((EXTI->PR & EXTI_Line0) && (EXTI->IMR & EXTI_Line0))
	{ 
		EXTI->PR = EXTI_Line0;
        
		if(g_gpioExtiManager[0].pIntCallback)
			g_gpioExtiManager[0].pIntCallback();
	}
}
void EXTI1_IRQHandler(void)
{
	if((EXTI->PR & EXTI_Line1) && (EXTI->IMR & EXTI_Line1))
	{ 
		EXTI->PR = EXTI_Line1;
        
		if(g_gpioExtiManager[1].pIntCallback)
			g_gpioExtiManager[1].pIntCallback();
	}
}
void EXTI2_IRQHandler(void)
{
	if((EXTI->PR & EXTI_Line2) && (EXTI->IMR & EXTI_Line2))
	{ 
		EXTI->PR = EXTI_Line2;
		
        if(g_gpioExtiManager[2].pIntCallback)
			g_gpioExtiManager[2].pIntCallback();
	}
}
void EXTI3_IRQHandler(void)
{
	if((EXTI->PR & EXTI_Line3) && (EXTI->IMR & EXTI_Line3))
	{ 
		EXTI->PR = EXTI_Line3;
		
        if(g_gpioExtiManager[3].pIntCallback)
			g_gpioExtiManager[3].pIntCallback();
	}
}
void EXTI4_IRQHandler(void)
{
	if((EXTI->PR & EXTI_Line4) && (EXTI->IMR & EXTI_Line4))
	{
		EXTI->PR = EXTI_Line4;
		
        if(g_gpioExtiManager[4].pIntCallback)
			g_gpioExtiManager[4].pIntCallback();
	}
}

void EXTI9_5_IRQHandler(void)
{
	if((EXTI->PR & EXTI_Line5) && (EXTI->IMR & EXTI_Line5))
	{
		EXTI->PR = EXTI_Line5;
		
        if(g_gpioExtiManager[5].pIntCallback)
			g_gpioExtiManager[5].pIntCallback();
	}

	if((EXTI->PR & EXTI_Line6) && (EXTI->IMR & EXTI_Line6))
	{ 
		EXTI->PR = EXTI_Line6;
		
        if(g_gpioExtiManager[6].pIntCallback)
			g_gpioExtiManager[6].pIntCallback();
	}

	if((EXTI->PR & EXTI_Line7) && (EXTI->IMR & EXTI_Line7))
	{ 
		EXTI->PR = EXTI_Line7;
		
        if(g_gpioExtiManager[7].pIntCallback)
			g_gpioExtiManager[7].pIntCallback();
	}

	if((EXTI->PR & EXTI_Line8) && (EXTI->IMR & EXTI_Line8))
	{ 
		EXTI->PR = EXTI_Line8;
		
        if(g_gpioExtiManager[8].pIntCallback)
			g_gpioExtiManager[8].pIntCallback();
	}

	if((EXTI->PR & EXTI_Line9) && (EXTI->IMR & EXTI_Line9))
	{ 
		EXTI->PR = EXTI_Line9;
		
        if(g_gpioExtiManager[9].pIntCallback)
			g_gpioExtiManager[9].pIntCallback();
	}
}

void EXTI15_10_IRQHandler(void)
{
	if((EXTI->PR & EXTI_Line10) && (EXTI->IMR & EXTI_Line10))
	{ 
		EXTI->PR = EXTI_Line10;
		
        if(g_gpioExtiManager[10].pIntCallback)
			g_gpioExtiManager[10].pIntCallback();
	}

	if((EXTI->PR & EXTI_Line11) && (EXTI->IMR & EXTI_Line11))
	{ 
		EXTI->PR = EXTI_Line11;
		
        if(g_gpioExtiManager[11].pIntCallback)
			g_gpioExtiManager[11].pIntCallback();
	}

	if((EXTI->PR & EXTI_Line12) && (EXTI->IMR & EXTI_Line12))
	{ 
		EXTI->PR = EXTI_Line12;
		
        if(g_gpioExtiManager[12].pIntCallback)
			g_gpioExtiManager[12].pIntCallback();
	}

	if((EXTI->PR & EXTI_Line13) && (EXTI->IMR & EXTI_Line13))
	{ 
		EXTI->PR = EXTI_Line13;
		
        if(g_gpioExtiManager[13].pIntCallback)
			g_gpioExtiManager[13].pIntCallback();
	}

	//if(EXTI_GetITStatus(EXTI_Line14) != RESET)
    if((EXTI->PR & EXTI_Line14) && (EXTI->IMR & EXTI_Line14))
	{ 
		//EXTI_ClearITPendingBit(EXTI_Line14);
        EXTI->PR = EXTI_Line14;
		
        if(g_gpioExtiManager[14].pIntCallback)
			g_gpioExtiManager[14].pIntCallback();
	}
    
	if((EXTI->PR & EXTI_Line15) && (EXTI->IMR & EXTI_Line15))
	{ 
		EXTI->PR = EXTI_Line15;
		
        if(g_gpioExtiManager[15].pIntCallback)
			g_gpioExtiManager[15].pIntCallback();
	}
}

void TIM1_BRK_IRQHandler(void)
{
	while (1)
		;
}
void TIM1_UP_IRQHandler(void)
{
	while (1)
		;
}
void TIM1_TRG_COM_IRQHandler(void)
{
	while (1)
		;
}
void TIM1_CC_IRQHandler(void)
{
	while (1)
		;
}

void TIM2_IRQHandler(void)
{ 
}

void TIM3_IRQHandler(void)
{
}

void TIM4_IRQHandler(void)   // if TIM4
{
	if((TIM4->SR & TIM_FLAG_Update) && (TIM4->DIER & TIM_FLAG_Update))
	{
        TIM4->SR = (uint16_t)~TIM_FLAG_Update;
	} 
}

void TIM5_IRQHandler(void)
{
	if((TIM5->SR & TIM_FLAG_Update) && (TIM5->DIER & TIM_FLAG_Update))
	{
		TIM5->SR = (uint16_t)~TIM_FLAG_Update;

		time_server_interrupt_proc5();
	}
}

void TIM6_IRQHandler(void)
{
	//if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    if((TIM6->SR & TIM_FLAG_Update) && (TIM6->DIER & TIM_FLAG_Update))
	{
		//TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
        TIM6->SR = (uint16_t)~TIM_FLAG_Update;

		if(g_time_server_proc_run6) {
			g_time_server_proc_run6();
		}
	}
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    g_nSysTickCount++;
}
