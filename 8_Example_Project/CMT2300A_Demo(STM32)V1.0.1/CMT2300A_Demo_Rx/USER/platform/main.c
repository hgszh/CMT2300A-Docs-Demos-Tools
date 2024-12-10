#include "typedefs.h"
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "system.h"
#include "time_server.h"
#include "interrupt_server.h"
#include "key_server.h"

#include "gpio_defs.h"
#include "led.h"
#include "lcd12864.h"
#include "buzzer.h"

#include "radio.h"

/*******************************************************
; Mode                      = Advanced
; Part Number               = CMT2300A
; Frequency                 = 433.920 MHz
; Xtal Frequency            = 26.0000 MHz
; Demodulation              = GFSK
; AGC                       = On
; Data Rate                 = 9.6 kbps
; Deviation                 = 20.0 kHz
; Tx Xtal Tol.              = 20 ppm
; Rx Xtal Tol.              = 20 ppm
; TRx Matching Network Type = 20 dBm
; Tx Power                  = +20 dBm
; Gaussian BT               = 0.5
; Bandwidth                 = Auto-Select kHz
; Data Mode                 = Packet
; Whitening                 = Disable
; Packet Type               = Fixed Length
; Payload Length            = 32
*********************************************************/

#define RF_RX_TIMEOUT    1000*60*60      //60min
#define RF_PACKET_SIZE   32               /* Define the payload size here */

static u8 g_rxBuffer[RF_PACKET_SIZE];   /* RF Rx buffer */
//static u8 g_txBuffer[RF_PACKET_SIZE];   /* RF Tx buffer */

char str[32];
u32 g_nRecvCount=0,g_nSendCount=0;


void Mcu_Init(void)
{
    /* system init */
    SystemInit();
    GPIO_Config();
    NVIC_Config();
    SystemTimerDelay_Config();
    Timer5_Config();

    lcd12864_init();
    buzzer_init();
    
    lcd12864_led_on();
}

u8 Radio_Recv_FixedLen(u8 pBuf[],u8 len)
{
#ifdef ENABLE_ANTENNA_SWITCH
	      if(CMT2300A_ReadGpio3())  /* Read INT2, PKT_DONE */
#else
				if(CMT2300A_ReadGpio1()) /* Read INT1, SYNC OK */
				{
				  /******/
				}
        if(CMT2300A_ReadGpio2())  /* Read INT2, PKT_DONE */
#endif	
		   {
//        if(CMT2300A_MASK_PKT_OK_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_FLAG))  /* Read PKT_OK flag */
				 {
						CMT2300A_GoStby();
						CMT2300A_ReadFifo(pBuf,len);
						CMT2300A_ClearRxFifo();
						CMT2300A_ClearInterruptFlags();
						CMT2300A_GoRx();
						
						return 1;
				 }
		   }

		return 0;
}

/* Main application entry point */
int main(void) //单收例程
{
	  u8 i=0;
	
    Mcu_Init();
    RF_Init();
	
	  if(FALSE==CMT2300A_IsExist()) {
        views_print_line(0, "CMT2300A not found!");
        while(1);
    }
    else {
        views_print_line(0, "CMT2300A ready RX");
    }
	
	  CMT2300A_GoStby();
	
		/* Must clear FIFO after enable SPI to read or write the FIFO */
    CMT2300A_EnableReadFifo();
		CMT2300A_ClearInterruptFlags();
		CMT2300A_ClearRxFifo();
  	CMT2300A_GoRx();
	
	  while(1)
		{
		   if(Radio_Recv_FixedLen(g_rxBuffer,RF_PACKET_SIZE))
			 {
				  g_nRfRxtimeoutCount=0;  //清除 Time Out计数
				  g_nRecvCount++;
          sprintf(str, "recv: %d", g_nRecvCount);
          views_print_line(2, str);
				 
			    for(i=0;i<RF_PACKET_SIZE;i++) //Clear Buff
				   g_rxBuffer[i]=0;
				 
			 }

        
       if(g_nRfRxtimeoutCount>RF_RX_TIMEOUT) //根据实际应用可以调整Time Out
			 {
				  g_nRfRxtimeoutCount=0;
					CMT2300A_GoSleep();
					CMT2300A_GoStby();
					CMT2300A_ClearInterruptFlags();
					CMT2300A_ClearRxFifo();
					CMT2300A_GoRx();
			 }				 
		}		
}



