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



#define RF_PACKET_SIZE   32               /* Define the payload size here */

//static u8 g_rxBuffer[RF_PACKET_SIZE];   /* RF Rx buffer */
static u8 g_txBuffer[RF_PACKET_SIZE];   /* RF Tx buffer */

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

u8 Radio_Send_FixedLen(const u8 pBuf[], u8 len)
{
	u32 delay;
	CMT2300A_GoStby();
	CMT2300A_ClearInterruptFlags();
	CMT2300A_ClearTxFifo();
	CMT2300A_EnableWriteFifo();
	
	CMT2300A_WriteFifo(pBuf, len); // 写 TX_FIFO
	CMT2300A_GoTx(); // 启动发送
	delay = 1000; //根据实际字节数可调整Time Out
	while(1)
	{
//    if(CMT2300A_MASK_TX_DONE_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_CLR1))  /* Read TX_DONE flag */
    if(CMT2300A_ReadGpio3())  /* Read INT2, TX_DONE */
		{
			CMT2300A_ClearInterruptFlags();	
			CMT2300A_GoSleep();
			buzzer_on();  //发送完成蜂鸣器响一次
			system_delay_ms(100);
			buzzer_off();
			
			g_nSendCount++; 
			sprintf(str, "send: %d", g_nSendCount);
      views_print_line(2, str);
			
			return 1; // 
		}
		
		if(delay==0) ////发送超时溢出，防止芯片死机，客户可根据实际开发情况调整超时时间
		{
			RF_Init(); 
			return 0; // 发送超时
		}
		
		system_delay_100us(2);
		delay--;
	}

	
}

/* Main application entry point */
int main(void)  //单发例程
{
    int i;

    for(i=0; i<RF_PACKET_SIZE; i++)  //填写发生数据
        g_txBuffer[i] = 1+i;
    
    Mcu_Init();
    RF_Init();
	
	  if(FALSE==CMT2300A_IsExist()) {
        views_print_line(0, "CMT2300A not found!");
        while(1);
    }
    else {
        views_print_line(0, "CMT2300A ready TX");
    }
	
	  while(1) 
		{
		  Radio_Send_FixedLen(g_txBuffer,RF_PACKET_SIZE);
			system_delay_ms(1000); 
		}
	  
}


