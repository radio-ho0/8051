#ifndef RA8803_H
#define RA8803_H
#include <mcs51/8052.h>

/*	端口定义	*/
#define LCD_RS	P2_0
#define LCD_WR	P2_1
#define LCD_RD	P2_2
#define LCD_CS	P2_3
#define LCD_DATA	P0


/*	参数设置	*/

//图层设置，双层模式，双层或关系，同时读写对Page1和Page2的DDRAM
#define	LCD_MAMR_VALUE	0xb3

#define	LCD_WLCR_VALUE	0xcd
#define	LCD_MISC_VALUE	0x03
#define	LCD_DWTR_VALUE	0
#define	LCD_DWBR_VALUE	239
#define	LCD_DWLR_VALUE	0
#define	LCD_DWRR_VALUE	39
#define	LCD_AWTR_VALUE	0
#define	LCD_AWBR_VALUE	239
#define	LCD_AWLR_VALUE	0
#define	LCD_AWRR_VALUE	39
#define	LCD_PNTR_VALUE	0x00
#define	LCD_CPXR_VALUE	0x00
#define	LCD_BGSG_VALUE	0x00
#define	LCD_CPYR_VALUE	0x00
#define	LCD_BGCM_VALUE	0x00
#define	LCD_EDCM_VALUE	0xef
#define	LCD_SCCR_VALUE	0x09
#define	LCD_FNCR_VALUE	0xa0
#define	LCD_FVHT_VALUE	0x0f
#define LCD_TPCR_VALUE	0xc8
#define LCD_TPSR_VALUE	0x3a
#define LCD_TPXR_VALUE	0x80
#define LCD_TPYR_VALUE	0x80
#define LCD_TPZR_VALUE	0x00
#define LCD_LCCR_VALUE	0x0f
#define	LCD_APSR_VALUE	0x10
#define	LCD_ADSR_VALUE	0x80
#define	LCD_WCCR_VALUE	0x6b

/*	函数声明	*/
void delay_us(unsigned int n);
void lcd_cmd_write (unsigned char cmdx , unsigned char cmdy);
void lcd_data_write(unsigned char datax);
unsigned char lcd_cmd_read(unsigned char cmdx);
unsigned char lcd_data_read(void);
void lcd_init(void);
void lcd_clear(void);
void lcd_cursor_xy(unsigned int xn,unsigned int yn);
#define	lcd_print_str(str)	lcd_print_str_zoom(str,0,0)
void lcd_print_str_zoom(unsigned char *str, unsigned char height, unsigned char width);

#endif
