#ifndef __CMT2300A_SPI_H
#define __CMT2300A_SPI_H

//Í·ÎÄ¼þ
#include "types.h"


//P2.5:CLK;P2.3:MISO;P2.4:MOSI
//P0.3:CSB;P2.6:FCSB    
#define	FCSB_H()    Gpio_SetIO(2,6,1)
#define	FCSB_L()    Gpio_SetIO(2,6,0)

#define	CSB_H()	    Gpio_SetIO(0,3,1)
#define	CSB_L()	    Gpio_SetIO(0,3,0)



//P0.1:GPIO1;P0.2:GPIO2
#define Read_CMT2300A_TXOK() (M0P_GPIO->P0IN & 0x02)
#define Read_CMT2300A_RXOK() (M0P_GPIO->P0IN & 0x04)


void Delay(u32 Cont);
void CMT2300A_SPI_Init(void);
u8 CMT2300A_SPI_Read(u8 Data);
void CMT2300A_SPI_Write(u8 adder,u8 Data);
void CMT2300A_ReadFifo(u8 *buf, u8 len);
void CMT2300A_WriteFifo(u8 *buf, u8 len);


#endif
