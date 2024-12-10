#ifndef __LCD12864_H
#define __LCD12864_H

#include "typedefs.h"
#include "stm32f10x_gpio.h"

#define LCD12864_DISP_OFF			0xAE
#define LCD12864_DISP_ON			0xAF
#define LCD12864_START_LINE(x)		(0x40 | ((x) & 0x3F))
#define LCD12864_PAGE_ADDR(x)		(0xB0 | ((x) & 0x0F))
#define LCD12864_COL_ADDR_H(x)		(0x10 | ((x) & 0x0F))
#define LCD12864_COL_ADDR_L(x)		(0x00 | ((x) & 0x0F))
#define LCD12864_ADC_NORMAL			0xA0
#define LCD12864_ADC_REVERSE		0xA1
#define LCD12864_DISP_NORMAL		0xA6
#define LCD12864_DISP_REVERSE		0xA7
#define LCD12864_DISP_NALL			0xA4
#define LCD12864_DISP_ALL			0xA5
#define LCD12864_BIAS_NORMAL		0xA2
#define LCD12864_BIAS_1_7			0xA3
#define LCD12864_SOFT_RESET			0xE2
#define LCD12864_OUTPUT_REVERSE		0xC0
#define LCD12864_OUTPUT_NORMAL		0xC8
#define LCD12864_POWER_CTRL_1		0x2C
#define LCD12864_POWER_CTRL_2		0x2E
#define LCD12864_POWER_CTRL_3		0x2F
#define LCD12864_R_FlAT_ADJUST(x)	(0x20 | ((x) & 0x07))
#define LCD12864_R_FINE_ADJUST		0x81
#define LCD12864_ICON_OFF			0xAC
#define LCD12864_ICON_ON			0xAD
#define LCD12864_NOP				0xE3

extern u8 g_lcd12864_write_data[8][128];
extern u8 g_lcd12864_curr_data[8][128];

void lcd12864_init(void);
void lcd12864_init_lcd(void);

void lcd12864_delay(void);
void lcd12864_delay_ms(u32 ms);

void lcd12864_send_cmd(u8 dat8);
void lcd12864_send_data(u8 dat8);
u8 lcd12864_read_data(u8 page, u8 column); 

void lcd12864_led_on(void);
void lcd12864_led_off(void);
void lcd12864_reset(void);
 
void lcd12864_set_address(u8 page, u8 column); 
void lcd12864_clear_screen(void);
void lcd12864_clear_line(u8 page, u8 column, u8 len); 
void lcd12864_full_display(void); 
void lcd12864_display_graphic_symbol(u8 page, u8 column, const u8 *p_data, u8 len);
void lcd12864_display_graphic_32x32(u8 page, u8 column, const u8 *dp);
void lcd12864_display_graphic_16x16(u8 page, u8 column, const u8 *dp);
void lcd12864_display_graphic_8x16(u8 page, u8 column, const u8 *dp);
void lcd12864_display_string_8x16(u8 page, u8 column, const u8 *text);
void lcd12864_display_string_6x8(u8 page, u8 column, const u8 *text);

void lcd12864_display_logo(void);
void lcd12864_set_auto_update(u8 is_auto);
void lcd12864_update_data(u8 is_update);
void lcd12864_draw_point(u8 x, u8 y);
void lcd12864_draw_line(u8 x1, u8 y1, u8 x2, u8 y2);

#endif

