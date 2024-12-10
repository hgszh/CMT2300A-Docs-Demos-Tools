#include "key_server.h"
#include "common.h"
#include "system.h"

#define KEY_INIT(key,gpio)  do {\
    (key)->gpioGroup     = gpio; \
    (key)->gpioPin       = gpio##_PIN; \
    (key)->status        = KEY_STATE_IDLE; \
    (key)->scanTimestamp = 0; \
    (key)->downTimestamp = 0; \
    (key)->result        = KEY_NONE; } while(0)

#define KEY_JITTER_TIME     20

tKeyScan g_key1;
tKeyScan g_key2;
tKeyScan g_key3;
tKeyScan g_key4;
tKeyScan g_key5;

void Key_Init(void)
{
    KEY_INIT(&g_key1, KEY1_GPIO);
    KEY_INIT(&g_key2, KEY2_GPIO);
    KEY_INIT(&g_key3, KEY3_GPIO);
    KEY_INIT(&g_key4, KEY4_GPIO);
    KEY_INIT(&g_key5, KEY5_GPIO);
}

void Key_ScanStatus(tKeyScan* pKey)
{
    u8 keyFlag = (pKey->gpioGroup->IDR & pKey->gpioPin) ?1 :0;
    
    switch(pKey->status)
    {
        case KEY_STATE_IDLE:
        {
            if(!keyFlag) {
                pKey->status        = KEY_STATE_FIRST_DOWN;
                pKey->scanTimestamp = g_nSysTickCount;
                pKey->result        = KEY_NONE;
            }
        }
        break;
        
        case KEY_STATE_FIRST_DOWN:
        {
            if(keyFlag) {
                pKey->status = KEY_STATE_IDLE;
            }
            else if(g_nSysTickCount - pKey->scanTimestamp >= KEY_JITTER_TIME) {
                pKey->status        = KEY_STATE_DOWN;
                pKey->scanTimestamp = g_nSysTickCount;
                pKey->downTimestamp = g_nSysTickCount;
                pKey->result       |= KEY_DOWN;
            }
        }
        break;
        
        case KEY_STATE_DOWN:
        {
            if(keyFlag) {
                pKey->status        = KEY_STATE_FIRST_UP;
                pKey->scanTimestamp = g_nSysTickCount;
            }
        }
        break;
        
        case KEY_STATE_FIRST_UP:
        {
            if(!keyFlag) {
                pKey->status = KEY_STATE_DOWN;
            }
            else if(g_nSysTickCount - pKey->scanTimestamp >= KEY_JITTER_TIME) {
                pKey->status        = KEY_STATE_UP;
                pKey->scanTimestamp = g_nSysTickCount;
                pKey->result       |= KEY_UP;
            }
        }
        break;
        
        case KEY_STATE_UP:
        {
            pKey->status = KEY_STATE_IDLE;
        }
        break;
    }
}

BOOL Key_IsDown(tKeyScan* pKey)
{
    BOOL ret = FALSE;
    
    Key_ScanStatus(pKey);
    
    if(KEY_DOWN & pKey->result) {
        ret = TRUE;
        pKey->result &= ~KEY_DOWN;
    }
    
    return ret;
}

BOOL Key_IsUp(tKeyScan* pKey)
{
    BOOL ret = FALSE;
    
    Key_ScanStatus(pKey);
    
    if(KEY_UP & pKey->result) {
        ret = TRUE;
        pKey->result &= ~KEY_UP;
    }
    
    return ret;
}

BOOL Key_IsLongDown(tKeyScan* pKey, u32 minInterval)
{
    BOOL ret = FALSE;
    
    Key_ScanStatus(pKey);
    
    if(KEY_STATE_DOWN==pKey->status && (g_nSysTickCount-pKey->scanTimestamp) > minInterval) {
        ret = TRUE;
        pKey->scanTimestamp = g_nSysTickCount;
    }
    
    return ret;
}

BOOL Key_IsLongUp(tKeyScan* pKey, u32 maxInterval)
{
    BOOL ret = FALSE;
    
    Key_ScanStatus(pKey);
    
    if((KEY_UP & pKey->result) && (g_nSysTickCount-pKey->downTimestamp) < maxInterval) {
        ret = TRUE;
        pKey->result &= ~KEY_UP;
    }
    
    return ret;
}
