/********************  (C) COPYRIGHT 2012  Cmostek Microelectronics Co., Ltd.  ********************
* File Name          : system.c
* Author             : IC Application Team
* Version            : V1.1.0
* Date               : 2010/5/29
* Description        : System control program
**************************************************************************************************/
#include "misc.h"

#include "typedefs.h"
#include "system.h"
#include "common.h"

void SystemTimerDelay_Config(void)  //sample as 72 is 72Mhz ,us TIM4,1us tick
{
    SysTick_Config(72*1000);    // 1ms as step
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}

void Timer5_Config()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    TIM_DeInit(TIM5);
        
    TIM_TimeBaseStructure.TIM_Period         = 1000 - 1;            // 1ms 
    TIM_TimeBaseStructure.TIM_Prescaler      = 72 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision  = TIM_CKD_DIV1;        // 采样分频
    TIM_TimeBaseStructure.TIM_CounterMode    = TIM_CounterMode_Up;  // 向上计数模式
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
        
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);                           // 清除溢出中断标志
    TIM_ARRPreloadConfig(TIM5, DISABLE);                            // 禁止ARR预装载缓冲器
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
     
    TIM_Cmd(TIM5, DISABLE);
}

void Timer6_Config(u32 nDataRate)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    TIM_DeInit(TIM6);

    nDataRate = (u32)1000000u / nDataRate;
    
    TIM_TimeBaseStructure.TIM_Period         = nDataRate - 1; 
    TIM_TimeBaseStructure.TIM_Prescaler      = 72 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision  = TIM_CKD_DIV1;         
    TIM_TimeBaseStructure.TIM_CounterMode    = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
        
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);                       
    TIM_ARRPreloadConfig(TIM6, DISABLE);                           
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
     
    TIM_Cmd(TIM6, DISABLE);
}

void DAC1_Config(const u16 adcValue[], u32 nAdcSize, u32 nAdcStep)
{
    DMA_InitTypeDef DMA_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    DAC_InitTypeDef DAC_InitStructure;
    
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Period        = nAdcStep - 1;
    TIM_TimeBaseStructure.TIM_Prescaler     = 72 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
    
    DMA_DeInit(DMA2_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&DAC->DHR12R1;
    DMA_InitStructure.DMA_MemoryBaseAddr        = (u32)adcValue;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize            = nAdcSize;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;//DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
    DMA_Init(DMA2_Channel3, &DMA_InitStructure);
    DMA_Cmd(DMA2_Channel3, ENABLE);
    
    DAC_StructInit(&DAC_InitStructure);
    DAC_DeInit();
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_DMACmd(DAC_Channel_1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    
    //DAC_SetChannel1Data(DAC_Align_12b_R, 4095);
}

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* as timestamp timer */
	NVIC_InitStructure.NVIC_IRQChannel 						= TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* as task timer */
	NVIC_InitStructure.NVIC_IRQChannel             			= TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority  			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd          			= ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* as rf_trans timer */
	NVIC_InitStructure.NVIC_IRQChannel             			= TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority  			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd          			= ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 						= USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel 						= EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel 						= EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
    NVIC_Init(&NVIC_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel 						= EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
}

void EXTI_Config(void)
{
}

void EXTI_ConfigOne(u32 nExitLine, EXTITrigger_TypeDef exitTrigger)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_ClearITPendingBit(nExitLine);

	EXTI_InitStructure.EXTI_Line 	= nExitLine;
    EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt; 
    EXTI_InitStructure.EXTI_Trigger = exitTrigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
