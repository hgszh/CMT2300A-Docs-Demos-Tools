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

#define RF_PACKET_SIZE 20 /* Define the payload size here */

static u8 g_rxBuffer[RF_PACKET_SIZE + 1]; /* RF Rx buffer */
static u8 g_txBuffer[RF_PACKET_SIZE];	  /* RF Tx buffer */

char str[32];
u32 g_nRecvCount = 0, g_nSendCount = 0;

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
void Recv_Err(void)
{
	CMT2300A_GoStby();
	CMT2300A_ClearRxFifo();
	CMT2300A_ClearInterruptFlags();
	CMT2300A_GoSleep();
	CMT2300A_GoStby();
	CMT2300A_GoRx();
}

u8 Radio_Recv_FixedLen(u8 pBuf[], u8 len)
{
#ifdef ENABLE_ANTENNA_SWITCH
	if (CMT2300A_ReadGpio3()) // 300H只能有GPIO3做接收PKT_DONE中断
#else
	if (CMT2300A_ReadGpio2()) //  PKT_DONE
#endif
	{
		if (CMT2300A_MASK_PKT_OK_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_FLAG)) /* 中断设置的时候PKT_DONE中断不代表一定收到一包数据，产生PKTD0NE的原因有四点具体请看芯片手册，所以此处在此查询是否Read PKT_OK flag */
		{
			CMT2300A_GoStby();
			CMT2300A_ReadFifo(pBuf, 1);
			if (pBuf[0] > 0 && pBuf[0] <= RF_PACKET_SIZE)
			{
				CMT2300A_ReadFifo(&pBuf[1], pBuf[0]);
				CMT2300A_ClearRxFifo();
				CMT2300A_ClearInterruptFlags();
				CMT2300A_GoSleep();
				return 1;
			}
			else
			{
				Recv_Err();
				return 0;
			}
		}
		else
		{
			Recv_Err();
			return 0;
		}
	}
	return 0;
}

u8 Radio_Send_FixedLen(const u8 pBuf[], u8 len)
{
	u8 wrLen = 0;
	u32 delay;
	CMT2300A_GoStby();
#ifdef ENABLE_ANTENNA_SWITCH
	/* Config interrupt */
	CMT2300A_ConfigInterrupt(
		CMT2300A_INT_SEL_PKT_DONE, /* Config INT1 */
		CMT2300A_INT_SEL_TX_DONE   /* 将INT2上的GPIO3设置为发射完成中断�Config INT2 */
	);
#endif
	CMT2300A_ClearInterruptFlags();
	CMT2300A_EnableWriteFifo();
	CMT2300A_ClearTxFifo();
	CMT2300A_SetPayloadLength(len);
	CMT2300A_WriteFifo(pBuf, len); // 写 TX_FIFO
	if (CMT2300A_GoTx() == 0)
		return 0; // 状态切换NG
	delay = 3000;
	while (1)
	{
#ifdef ENABLE_ANTENNA_SWITCH
		if (CMT2300A_ReadGpio3()) // 300H只能有GPIO3做发射完成中断
#else
		if (CMT2300A_ReadGpio1()) // TX_DONE
#endif
		// if(CMT2300A_MASK_TX_DONE_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_CLR1))  /* Read TX_DONE flag */
		{
			CMT2300A_GoStby();
			CMT2300A_ClearInterruptFlags();
			CMT2300A_GoSleep();
			return 1; //
		}

		if (delay == 0) // 超时溢出
		{
			CMT2300A_GoStby();
			CMT2300A_ClearInterruptFlags();
			CMT2300A_GoSleep();
			return 0; // 发送超时
		}
		system_delay_100us(2);
		delay--;
	}
}

#if 0 // 1为单发   0为单收
/* Main application entry point */
int main(void) // 单发例程
{
	int i;
	g_nRecvCount = 0;

	for (i = 0; i < RF_PACKET_SIZE; i++)
		g_txBuffer[i] = 1 + i;

	Mcu_Init();
	RF_Init();

	if (FALSE == CMT2300A_IsExist())
	{
		views_print_line(0, "CMT2300A not found!");
		led_on(LED_INDEX3);
		while (1)
			;
	}
	else
	{
		views_print_line(0, "CMT2300A ready TX");
	}

	while (1)
	{
		if (Radio_Send_FixedLen(g_txBuffer, RF_PACKET_SIZE) == 1)
		{
			g_nRecvCount++;
			sprintf(str, "Tx: %d", g_nRecvCount);
			views_print_line(2, str);
		}
		else
		{
			RF_Init(); // 异常处理
		}
		system_delay_ms(2000);
	}
}
#else
/* Main application entry point */
int main(void) // 单收例程
{
	u8 i = 0;
	Mcu_Init();

Status_err:
	RF_Init();
	if (FALSE == CMT2300A_IsExist())
	{
		views_print_line(0, "CMT2300A not found!");
		led_on(LED_INDEX3);
		while (1)
			;
	}
	else
	{
		views_print_line(0, "CMT2300A ready");
	}
	CMT2300A_GoStby();
#ifdef ENABLE_ANTENNA_SWITCH
	/* Config interrupt */
	CMT2300A_ConfigInterrupt(
		CMT2300A_INT_SEL_TX_DONE, /* Config INT1 */
		CMT2300A_INT_SEL_PKT_DONE /* 将INT2上的GPIO3设置为接收完成中断�Config INT2 */
	);
#endif
	/* Must clear FIFO after enable SPI to read or write the FIFO */
	CMT2300A_EnableReadFifo();
	CMT2300A_ClearInterruptFlags();
	CMT2300A_ClearRxFifo();
	if (FALSE == CMT2300A_GoRx())
	{
		views_print_line(1, "CMT2300A GORX_err!");
		goto Status_err;
	}

	while (1)
	{
		if (Radio_Recv_FixedLen(g_rxBuffer, RF_PACKET_SIZE))
		{
			buzzer_on(); // 发送完成蜂鸣器响一次
			g_nRecvCount++;
			sprintf(str, "recv: %d", g_nRecvCount);
			views_print_line(2, str);
			system_delay_ms(10);
			for (i = 0; i < RF_PACKET_SIZE + 1; i++)
				g_rxBuffer[i] = 0;
			buzzer_off();
			CMT2300A_GoStby();
			CMT2300A_GoRx();
		}
		system_delay_10us(2);
	}
}
#endif
