#ifndef __CMT_SPI3_H
#define __CMT_SPI3_H

#include "typedefs.h"
#include "stm32f10x_conf.h"

__inline void cmt_spi3_delay(void);
__inline void cmt_spi3_delay_us(void);

void cmt_spi3_init(void);

void cmt_spi3_send(u8 data8);
u8 cmt_spi3_recv(void);

void cmt_spi3_write(u8 addr, u8 dat);
void cmt_spi3_read(u8 addr, u8* p_dat);

void cmt_spi3_write_fifo(const u8* p_buf, u16 len);
void cmt_spi3_read_fifo(u8* p_buf, u16 len);

#endif
