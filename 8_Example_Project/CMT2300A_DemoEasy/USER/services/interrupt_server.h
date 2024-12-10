#ifndef __INTERRUPT_SERVER_H
#define __INTERRUPT_SERVER_H

#include "typedefs.h"


typedef void (*pIntSrvCallback)(void);

typedef struct _sGpioExtiManager {
	GPIO_TypeDef*   pGpioPort;
	uint16_t        nGpioPin;
    EXTITrigger_TypeDef nExitTrigger;
	pIntSrvCallback     pIntCallback;
	
} tGpioExtiManager;

typedef struct _sGpioExtiPortMap {
	GPIO_TypeDef*   pGpioPort;
	u8              nGpioPortSource;
	
} tGpioExtiPortMap;

typedef struct _sGpioExtiPinMap {
	u8              nIndex;
	uint16_t        nGpioPin;
	u8              nGpioPinSource;
	u32             nExtiLine;
	
} tGpioExtiPinMap;

#define GPIO_EXIT_MANAGER_SIZE	16

extern tGpioExtiManager g_gpioExtiManager[GPIO_EXIT_MANAGER_SIZE];

void IntSrv_Init(void);
void IntSrv_Register(GPIO_TypeDef* pGpioPort, uint16_t nPin, EXTITrigger_TypeDef nExtiTrigger, pIntSrvCallback pIntCallback);
void IntSrv_UnRegister(GPIO_TypeDef* pGpioPort, uint16_t nPin);
void IntSrv_Backup(void);
void IntSrv_Restore(void);

const tGpioExtiPortMap* IntSrc_FindGpioExtiPortMap(GPIO_TypeDef* pGpioPort);
const tGpioExtiPinMap* IntSrc_FindGpioExtiPinMap(uint16_t nGpioPin);

#endif
