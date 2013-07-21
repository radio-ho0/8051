#ifndef LCD_12864_H
#define LCD_12864_H

#ifdef LCD_DEBUG
	#define ASSERT(condition) if(condition) return
#else
	#define ASSERT(condition)
#endif

#ifndef __delay_us
void __delay_us(long iTime);
#endif

/*	函数声明	*/
//基本功能函数
void delay_10us(unsigned char n);
void lcd_wait_busy();
void lcd_write_cmd(unsigned char cmd);
void lcd_write_data(unsigned char a);
void lcd_clr(void);
void lcd_expand_switch(unsigned char value);
void lcd_pos_set(unsigned char x,unsigned char y);
void lcd_init(void);

//定起点显示字串、数值、图像
void lcd_print_strxy(unsigned char *str,unsigned char x, unsigned char y);
void lcd_print_numxy(long int num,unsigned char x, unsigned char y);
void lcd_print_picxy(unsigned short data, unsigned char x, unsigned char y);

void lcd_negative(unsigned char action, unsigned char x, unsigned char y);
void lcd_clr_pic(void);
void lcd_roll(unsigned char action);
void lcd_set_roll_add(unsigned char x);
void lcd_negative(unsigned char action,unsigned char x, unsigned char y);

#endif
