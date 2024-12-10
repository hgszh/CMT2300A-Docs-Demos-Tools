/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, CMOSTEK SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (C) CMOSTEK SZ.
 */

/*!
 * @file    radio.c
 * @brief   Generic radio handlers
 *
 * @version 1.2
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */
 
#include "radio.h"
#include "cmt2300a_params.h"

#include <string.h>

static EnumRFStatus g_nNextRFState = RF_STATE_IDLE;
static u8* g_pRxBuffer = NULL;
static u8* g_pTxBuffer = NULL;
static u16 g_nRxLength = 0;
static u16 g_nTxLength = 0;

static u32 g_nRxTimeout = INFINITE;
static u32 g_nTxTimeout = INFINITE;
static u32 g_nRxTimeCount = 0;
static u32 g_nTxTimeCount = 0;

static u8 g_nInterrutFlags = 0;

void RF_Init(void)
{
    u8 tmp;
    
    CMT2300A_InitGpio();
	CMT2300A_Init();
    
    /* Config registers */
    CMT2300A_ConfigRegBank(CMT2300A_CMT_BANK_ADDR       , g_cmt2300aCmtBank       , CMT2300A_CMT_BANK_SIZE       );
    CMT2300A_ConfigRegBank(CMT2300A_SYSTEM_BANK_ADDR    , g_cmt2300aSystemBank    , CMT2300A_SYSTEM_BANK_SIZE    );
    CMT2300A_ConfigRegBank(CMT2300A_FREQUENCY_BANK_ADDR , g_cmt2300aFrequencyBank , CMT2300A_FREQUENCY_BANK_SIZE );
    CMT2300A_ConfigRegBank(CMT2300A_DATA_RATE_BANK_ADDR , g_cmt2300aDataRateBank  , CMT2300A_DATA_RATE_BANK_SIZE );
    CMT2300A_ConfigRegBank(CMT2300A_BASEBAND_BANK_ADDR  , g_cmt2300aBasebandBank  , CMT2300A_BASEBAND_BANK_SIZE  );
    CMT2300A_ConfigRegBank(CMT2300A_TX_BANK_ADDR        , g_cmt2300aTxBank        , CMT2300A_TX_BANK_SIZE        );
    
    // xosc_aac_code[2:0] = 2
    tmp = (~0x07) & CMT2300A_ReadReg(CMT2300A_CUS_CMT10);
    CMT2300A_WriteReg(CMT2300A_CUS_CMT10, tmp|0x02);
    
	RF_Config();
}

void RF_Config(void)
{
#ifdef ENABLE_ANTENNA_SWITCH
    /* If you enable antenna switch, GPIO1/GPIO2 will output RX_ACTIVE/TX_ACTIVE,
       and it can't output INT1/INT2 via GPIO1/GPIO2 */
    CMT2300A_EnableAntennaSwitch(0);
    
#else
    /* Config GPIOs */
    CMT2300A_ConfigGpio(
        CMT2300A_GPIO1_SEL_INT1 | /* INT1 > GPIO1 */
        CMT2300A_GPIO2_SEL_INT2 | /* INT2 > GPIO2 */
        CMT2300A_GPIO3_SEL_DOUT
        );
    
    /* Config interrupt */
    CMT2300A_ConfigInterrupt(
        CMT2300A_INT_SEL_TX_DONE, /* Config INT1 */
        CMT2300A_INT_SEL_PKT_OK   /* Config INT2 */
        );
#endif

    /* Enable interrupt */
    CMT2300A_EnableInterrupt(
        CMT2300A_MASK_TX_DONE_EN  |
        CMT2300A_MASK_PREAM_OK_EN |
        CMT2300A_MASK_SYNC_OK_EN  |
        CMT2300A_MASK_NODE_OK_EN  |
        CMT2300A_MASK_CRC_OK_EN   |
        CMT2300A_MASK_PKT_DONE_EN
        );
    
    /* Disable low frequency OSC calibration */
    CMT2300A_EnableLfosc(FALSE);
    
    /* Use a single 64-byte FIFO for either Tx or Rx */
    //CMT2300A_EnableFifoMerge(TRUE);
    
    //CMT2300A_SetFifoThreshold(16);
    
    /* This is optional, only needed when using Rx fast frequency hopping */
    /* See AN142 and AN197 for details */
    //CMT2300A_SetAfcOvfTh(0x27);
    
    /* Go to sleep for configuration to take effect */
    CMT2300A_GoSleep();
}

void RF_SetStatus(EnumRFStatus nStatus)
{
    g_nNextRFState = nStatus;
}

EnumRFStatus RF_GetStatus(void)
{
    return g_nNextRFState;
}

u8 RF_GetInterruptFlags(void)
{
    return g_nInterrutFlags;
}

void RF_StartRx(u8 buf[], u16 len, u32 timeout)
{
    g_pRxBuffer = buf;
    g_nRxLength = len;
    g_nRxTimeout = timeout;
    
    memset(g_pRxBuffer, 0, g_nRxLength);
    
    g_nNextRFState = RF_STATE_RX_START;
}

void RF_StartTx(u8 buf[], u16 len, u32 timeout)
{
    g_pTxBuffer = buf;
    g_nTxLength = len;
    g_nTxTimeout = timeout;
    
    g_nNextRFState = RF_STATE_TX_START;
}

EnumRFResult RF_Process(void)
{
    EnumRFResult nRes = RF_BUSY;
    
    switch(g_nNextRFState) 
    {
    case RF_STATE_IDLE:
    {
        nRes = RF_IDLE;
        break;
    }
    
    case RF_STATE_RX_START:
    {
        CMT2300A_GoStby();
        CMT2300A_ClearInterruptFlags();
        
        /* Must clear FIFO after enable SPI to read or write the FIFO */
        CMT2300A_EnableReadFifo();
        CMT2300A_ClearRxFifo();
        
        if(FALSE==CMT2300A_GoRx())
            g_nNextRFState = RF_STATE_ERROR;
        else
            g_nNextRFState = RF_STATE_RX_WAIT;
        
        g_nRxTimeCount = CMT2300A_GetTickCount();
        
        break;
    }
    
    case RF_STATE_RX_WAIT:
    {
#ifdef ENABLE_ANTENNA_SWITCH
        if(CMT2300A_MASK_PKT_OK_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_FLAG))  /* Read PKT_OK flag */
#else
        if(CMT2300A_ReadGpio2())  /* Read INT2, PKT_OK */
#endif
        {
            g_nNextRFState = RF_STATE_RX_DONE;
        }
        
        if( (INFINITE != g_nRxTimeout) && ((g_nSysTickCount-g_nRxTimeCount) > g_nRxTimeout) )
            g_nNextRFState = RF_STATE_RX_TIMEOUT;
        
        break;
    }
    
    case RF_STATE_RX_DONE:
    {
        CMT2300A_GoStby();

        /* The length need be smaller than 32 */
        CMT2300A_ReadFifo(g_pRxBuffer, g_nRxLength);

        g_nInterrutFlags = CMT2300A_ClearInterruptFlags();
            
        CMT2300A_GoSleep();
        
        g_nNextRFState = RF_STATE_IDLE;
        nRes = RF_RX_DONE;
        break;
    }
    
    case RF_STATE_RX_TIMEOUT:
    {
        CMT2300A_GoSleep();
        
        g_nNextRFState = RF_STATE_IDLE;
        nRes = RF_RX_TIMEOUT;
        break;
    }
    
    case RF_STATE_TX_START:
    {
        CMT2300A_GoStby();
        CMT2300A_ClearInterruptFlags();
        
        /* Must clear FIFO after enable SPI to read or write the FIFO */
        CMT2300A_EnableWriteFifo();
        CMT2300A_ClearTxFifo();
        
        /* The length need be smaller than 32 */
        CMT2300A_WriteFifo(g_pTxBuffer, g_nTxLength);
        
        if( 0==(CMT2300A_MASK_TX_FIFO_NMTY_FLG & CMT2300A_ReadReg(CMT2300A_CUS_FIFO_FLAG)) )
            g_nNextRFState = RF_STATE_ERROR;

        if(FALSE==CMT2300A_GoTx())
            g_nNextRFState = RF_STATE_ERROR;
        else
            g_nNextRFState = RF_STATE_TX_WAIT;
        
        g_nTxTimeCount = CMT2300A_GetTickCount();
        
        break;
    }
        
    case RF_STATE_TX_WAIT:
    {
#ifdef ENABLE_ANTENNA_SWITCH
        if(CMT2300A_MASK_TX_DONE_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_CLR1))  /* Read TX_DONE flag */
#else
        if(CMT2300A_ReadGpio1())  /* Read INT1, TX_DONE */
#endif
        {
            g_nNextRFState = RF_STATE_TX_DONE;
        }
        
        if( (INFINITE != g_nTxTimeout) && ((g_nSysTickCount-g_nTxTimeCount) > g_nTxTimeout) )
            g_nNextRFState = RF_STATE_TX_TIMEOUT;
            
        break;
    }
            
    case RF_STATE_TX_DONE:
    {
        CMT2300A_ClearInterruptFlags();
        CMT2300A_GoSleep();

        g_nNextRFState = RF_STATE_IDLE;
        nRes = RF_TX_DONE;
        break;
    }
    
    case RF_STATE_TX_TIMEOUT:
    {
        CMT2300A_GoSleep();
        
        g_nNextRFState = RF_STATE_IDLE;
        nRes = RF_TX_TIMEOUT;
        break;
    }
    
    case RF_STATE_ERROR:
    {
        CMT2300A_SoftReset();
        CMT2300A_DelayMs(20);
        
        CMT2300A_GoStby();
        RF_Config();
        
        g_nNextRFState = RF_STATE_IDLE;
        nRes = RF_ERROR;
        break;
    }
    
    default:
        break;
    }
    
    return nRes;
}

