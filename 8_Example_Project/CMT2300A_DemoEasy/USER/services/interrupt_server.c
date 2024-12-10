#include "interrupt_server.h"
#include "common.h"
#include "system.h"

#include <string.h>

// PE8  KEY1_GPIO
// PE9  KEY2_GPIO
// PE10 KEY3_GPIO
// PE11 KEY4_GPIO
// PE12 KEY5_GPIO

tGpioExtiManager g_gpioExtiManager[GPIO_EXIT_MANAGER_SIZE];
tGpioExtiManager g_gpioExtiManagerBackup[GPIO_EXIT_MANAGER_SIZE];

static const tGpioExtiPortMap g_gpioExtiPortMap[] = {
	{ GPIOA, GPIO_PortSourceGPIOA },
    { GPIOB, GPIO_PortSourceGPIOB },
    { GPIOC, GPIO_PortSourceGPIOC },
    { GPIOD, GPIO_PortSourceGPIOD },
    { GPIOE, GPIO_PortSourceGPIOE },
    { GPIOF, GPIO_PortSourceGPIOF },
	{ GPIOG, GPIO_PortSourceGPIOG },
	
};

static const tGpioExtiPinMap g_gpioExtiPinMap[] = {
	{ 0  , GPIO_Pin_0  , GPIO_PinSource0  , EXTI_Line0  },
    { 1  , GPIO_Pin_1  , GPIO_PinSource1  , EXTI_Line1  },
    { 2  , GPIO_Pin_2  , GPIO_PinSource2  , EXTI_Line2  },
    { 3  , GPIO_Pin_3  , GPIO_PinSource3  , EXTI_Line3  },
    { 4  , GPIO_Pin_4  , GPIO_PinSource4  , EXTI_Line4  },
    { 5  , GPIO_Pin_5  , GPIO_PinSource5  , EXTI_Line5  },
    { 6  , GPIO_Pin_6  , GPIO_PinSource6  , EXTI_Line6  },
    { 7  , GPIO_Pin_7  , GPIO_PinSource7  , EXTI_Line7  },
    { 8  , GPIO_Pin_8  , GPIO_PinSource8  , EXTI_Line8  },
    { 9  , GPIO_Pin_9  , GPIO_PinSource9  , EXTI_Line9  },
    { 10 , GPIO_Pin_10 , GPIO_PinSource10 , EXTI_Line10 },
    { 11 , GPIO_Pin_11 , GPIO_PinSource11 , EXTI_Line11 },
    { 12 , GPIO_Pin_12 , GPIO_PinSource12 , EXTI_Line12 },
    { 13 , GPIO_Pin_13 , GPIO_PinSource13 , EXTI_Line13 },
    { 14 , GPIO_Pin_14 , GPIO_PinSource14 , EXTI_Line14 },
	{ 15 , GPIO_Pin_15 , GPIO_PinSource15 , EXTI_Line15 },
	
};

void IntSrv_Init(void)
{
	memset(g_gpioExtiManager, 0, sizeof(g_gpioExtiManager));
	memset(g_gpioExtiManagerBackup, 0, sizeof(g_gpioExtiManagerBackup));
}

void IntSrv_Register(GPIO_TypeDef* pGpioPort, uint16_t nPin, EXTITrigger_TypeDef nExtiTrigger, pIntSrvCallback pIntCallback)
{
	const tGpioExtiPortMap* pGpioExtiPortMap = IntSrc_FindGpioExtiPortMap(pGpioPort);
	const tGpioExtiPinMap* pGpioExtiPinMap = IntSrc_FindGpioExtiPinMap(nPin);
	
	if(pGpioExtiPinMap && pGpioExtiPortMap)
    {
        EXTI_InitTypeDef EXTI_InitStructure;
        
        g_gpioExtiManager[pGpioExtiPinMap->nIndex].pGpioPort    = pGpioPort;
        g_gpioExtiManager[pGpioExtiPinMap->nIndex].nGpioPin     = nPin;
        g_gpioExtiManager[pGpioExtiPinMap->nIndex].nExitTrigger = nExtiTrigger;
        g_gpioExtiManager[pGpioExtiPinMap->nIndex].pIntCallback = pIntCallback;
        
        EXTI_InitStructure.EXTI_Line 	= pGpioExtiPinMap->nExtiLine;
        EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt; 
        EXTI_InitStructure.EXTI_Trigger = nExtiTrigger;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        
        GPIO_EXTILineConfig(pGpioExtiPortMap->nGpioPortSource, pGpioExtiPinMap->nGpioPinSource);
        EXTI_ClearITPendingBit(pGpioExtiPinMap->nExtiLine);
        EXTI_Init(&EXTI_InitStructure);
    }
}

void IntSrv_UnRegister(GPIO_TypeDef* pGpioPort, uint16_t nPin)
{
    const tGpioExtiPortMap* pGpioExtiPortMap = IntSrc_FindGpioExtiPortMap(pGpioPort);
	const tGpioExtiPinMap* pGpioExtiPinMap = IntSrc_FindGpioExtiPinMap(nPin);
	
	if(pGpioExtiPinMap && pGpioExtiPortMap)
    {
        if( g_gpioExtiManager[pGpioExtiPinMap->nIndex].pGpioPort == pGpioPort && 
            g_gpioExtiManager[pGpioExtiPinMap->nIndex].nGpioPin == nPin )
        {
            EXTI_InitTypeDef EXTI_InitStructure;
            
            g_gpioExtiManager[pGpioExtiPinMap->nIndex].pGpioPort    = 0;
            g_gpioExtiManager[pGpioExtiPinMap->nIndex].nGpioPin     = 0;
            g_gpioExtiManager[pGpioExtiPinMap->nIndex].pIntCallback = 0;
        
            EXTI_InitStructure.EXTI_Line 	= pGpioExtiPinMap->nExtiLine;
            EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt; 
            EXTI_InitStructure.EXTI_LineCmd = DISABLE;
            
            EXTI_Init(&EXTI_InitStructure);
        }
    }
}

void IntSrv_Backup(void)
{
    memcpy(g_gpioExtiManagerBackup, g_gpioExtiManager, sizeof(g_gpioExtiManager));
}

void IntSrv_Restore(void)
{
    int i;
    for(i=0; i<GPIO_EXIT_MANAGER_SIZE; i++)
    {
        if(0!=memcmp(&g_gpioExtiManager[i], &g_gpioExtiManagerBackup[i], sizeof(tGpioExtiManager)))
        {
            IntSrv_Register(
                g_gpioExtiManagerBackup[i].pGpioPort, 
                g_gpioExtiManagerBackup[i].nGpioPin,
                g_gpioExtiManagerBackup[i].nExitTrigger,
                g_gpioExtiManagerBackup[i].pIntCallback );
        }
    }
}

const tGpioExtiPortMap* IntSrc_FindGpioExtiPortMap(GPIO_TypeDef* pGpioPort)
{
	int i;
	
	for(i=0; i<sizeof(g_gpioExtiPortMap)/sizeof(tGpioExtiPortMap); i++) {
		if(g_gpioExtiPortMap[i].pGpioPort == pGpioPort)
			return &g_gpioExtiPortMap[i];
	}
	
	return 0;
}

const tGpioExtiPinMap* IntSrc_FindGpioExtiPinMap(uint16_t nGpioPin)
{
	int i;
	
	for(i=0; i<sizeof(g_gpioExtiPinMap)/sizeof(tGpioExtiPinMap); i++) {
		if(g_gpioExtiPinMap[i].nGpioPin == nGpioPin)
			return &g_gpioExtiPinMap[i];
	}
	
	return 0;
}
