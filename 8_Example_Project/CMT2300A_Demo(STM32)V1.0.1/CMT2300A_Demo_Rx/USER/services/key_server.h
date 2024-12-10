#ifndef __KEY_SERVER_H
#define __KEY_SERVER_H

#include "typedefs.h"
#include "gpio_defs.h"

typedef enum {
    KEY_STATE_IDLE       = 0x00,
    KEY_STATE_FIRST_DOWN = 0x01,
    KEY_STATE_FIRST_UP   = 0x02,
    KEY_STATE_DOWN       = 0x03,
    KEY_STATE_UP         = 0x04,
    
} tKeyStatus;

typedef enum {
    KEY_NONE = 0x00,
    KEY_DOWN = 0x01,
    KEY_UP   = 0x02,
    
} tKeyResult;

typedef struct _sKeyScan {
    GPIO_TypeDef* gpioGroup;
    u16           gpioPin;
    tKeyStatus    status;
    u32           scanTimestamp;
    u32           downTimestamp;
    tKeyResult    result;
    
} tKeyScan;

extern tKeyScan g_key1;
extern tKeyScan g_key2;
extern tKeyScan g_key3;
extern tKeyScan g_key4;
extern tKeyScan g_key5;

void Key_Init(void);
void Key_ScanStatus(tKeyScan* pKey);
BOOL Key_IsDown(tKeyScan* pKey);
BOOL Key_IsUp(tKeyScan* pKey);
BOOL Key_IsLongDown(tKeyScan* pKey, u32 minInterval);
BOOL Key_IsLongUp(tKeyScan* pKey, u32 maxInterval);

#endif
