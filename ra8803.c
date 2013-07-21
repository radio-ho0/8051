#include "ra8803.h"


/*	寄存器地址定义	*/

//液晶全局控制器
#define LCD_WLCR	0x00	//Whole chip LCD Controller Register
#define LCD_MISC	0x01	//Mosc. Register
#define	LCD_ASPR	0x02
#define	LCD_ADSR	0x03
#define	LCD_WCCR	0x10
#define	LCD_MAMR	0x12

#define	LCD_AWRR	0x20
#define	LCD_AWBR	0x30
#define	LCD_AWLR	0x40
#define	LCD_AWTR	0x50
#define	LCD_DWRR	0x21
#define	LCD_DWBR	0x31
#define	LCD_DWLR	0x41
#define	LCD_DWTR	0x51

#define	LCD_CPXR	0x60
#define	LCD_BGSG	0x61
#define	LCD_CPYR	0x70
#define	LCD_BGCM	0x71
#define	LCD_EDCM	0x72
#define	LCD_SCCR	0x90

#define LCD_LCCR	0xd0	//LCD Contrast Control Register
#define	LCD_TPCR	0xc0	//Touch Panel Control Register
#define	LCD_TPSR	0xc1	//Touch Panel Status Register
#define LCD_TPXR	0xc8	//Touch Panel Segment High Byte Data Register
#define LCD_TPYR	0xc9	//Touch Panel Common High Byte Data Register
#define LCD_TPZR	0xca	//Touch Panel Segment&Common Low Byte Data Register
#define	LCD_PNTR	0xe0
#define	LCD_FNCR	0xf0
#define	LCD_FVHT	0xf1


#ifndef FUNCTION_DELAY_US
#define FUNCTION_DELAY_US
void delay_us(unsigned int n)
{
/*
	延时微函数
参数：
	n:微秒数
*/
	while(n--);
}
#endif

void lcd_cmd_write (unsigned char addr , unsigned char db){
/*
	写指令寄存器
参数：
	cmd：指令寄存器地址
	cmd_data：要写入的值

说明：
	写指令过程：先写入寄存器地址，再写入寄存器数据
	RD=L，读信号无效；	RS=L，指令寄存器；	WR↑写入
	
*/
	LCD_DATA=addr;

	LCD_CS=0;
	LCD_RD=1;	LCD_RS=0;
	LCD_WR=0;	LCD_WR=1;	//WR上升沿写入
	LCD_RS=1;
	LCD_CS=1;
	
	LCD_DATA=db;
	LCD_CS=0;
	LCD_RD=1;	LCD_RS=0;
	LCD_WR=0;	LCD_WR=1;
	LCD_RS=1;
	LCD_CS=1;
}


unsigned char lcd_cmd_read(unsigned char addr)
{
/*
	读指令寄存器
参数：
	addr：指令寄存器地址
返回：
	对应的寄存器值
说明：
	读指令寄存器过程：
		先写入寄存器地址：	RD=L，读信号无效；	RS=L，指令寄存器；	WR↑写入
		再读出寄存器数据：	WR=L，写信号无效；	RS=L，指令寄存器；	RD↓写入
*/
	unsigned char db;
	//写地址
	LCD_DATA=addr;
	LCD_CS=0;
	LCD_RD=1;	LCD_RS=0;
	LCD_WR=0;
	LCD_WR=1;	LCD_RS=1;	LCD_CS=1;

	//读数据并返回
	LCD_DATA=0xff;
	LCD_CS=0;
	LCD_WR=1;
	LCD_RS=0;
	LCD_RD=0;
	db=LCD_DATA;
	LCD_RD=1;
	LCD_RS=1;
	LCD_CS=1;

	return(db);
}




void lcd_data_write(unsigned char db)
{
/*
	写显示数据
参数：
	db：要显示的数据
说明：
	RD=L，读信号无效；	RS=H，数据寄存器；	WR↑写入
*/
	LCD_DATA=db;
	LCD_CS=0;	LCD_RD=1;	LCD_RS=1;
	LCD_WR=0;	LCD_WR=1;	//WR上升沿写入
	LCD_RS=0;	LCD_CS=1;
}


unsigned char lcd_data_read(void)
{
/*
	读显示数据
参数：
	db：要显示的数据
说明：
	RD=L，读信号无效；	RS=H，数据寄存器；	WR↑写入
*/
	unsigned char db;
	LCD_DATA=0xff;
	LCD_CS=0;
	LCD_WR=1;
	LCD_RS=1;
	LCD_RD=0;
	db=LCD_DATA;
	LCD_RD=1;
	LCD_RS=0;
	LCD_CS=1;

	return(db);
}

void lcd_init(void)
{
/*
	液晶初始化
*/
	lcd_cmd_write(LCD_WLCR,LCD_WLCR_VALUE);//WLCR 正常模式；文字模式；屏幕开启；不闪烁；不反白
	/*
		杂项设置
	晶振（低两位）低两位为00~11分别对应3、4、8、12MHz
	*/
	lcd_cmd_write(LCD_MISC,LCD_MISC_VALUE);
	lcd_cmd_write(LCD_ASPR,LCD_APSR_VALUE);//APSR
	lcd_cmd_write(LCD_ADSR,LCD_ADSR_VALUE);//ADSR 平移方向动控制
	lcd_cmd_write(LCD_WCCR,LCD_WCCR_VALUE);//WCCR 正常显示，不加粗
	lcd_cmd_write(LCD_MAMR,LCD_MAMR_VALUE);//图层设置

	//显示及活动窗口范围初始化
	lcd_cmd_write(LCD_AWTR,LCD_AWTR_VALUE);
	lcd_cmd_write(LCD_AWBR,LCD_AWBR_VALUE);
	lcd_cmd_write(LCD_AWLR,LCD_AWLR_VALUE);
	lcd_cmd_write(LCD_AWRR,LCD_AWRR_VALUE);
	lcd_cmd_write(LCD_DWTR,LCD_DWTR_VALUE);
	lcd_cmd_write(LCD_DWBR,LCD_DWBR_VALUE);
	lcd_cmd_write(LCD_DWLR,LCD_DWLR_VALUE);
	lcd_cmd_write(LCD_DWRR,LCD_DWRR_VALUE);


	
	lcd_cmd_write(LCD_CPXR,LCD_CPXR_VALUE);
	lcd_cmd_write(LCD_BGSG,LCD_BGSG_VALUE);
	lcd_cmd_write(LCD_CPYR,LCD_CPYR_VALUE);
	lcd_cmd_write(LCD_BGCM,LCD_BGCM_VALUE);
	lcd_cmd_write(LCD_EDCM,LCD_EDCM_VALUE);
	lcd_cmd_write(LCD_SCCR,LCD_SCCR_VALUE);

	//触摸屏控制、状态寄存器
	lcd_cmd_write(LCD_TPCR,LCD_TPCR_VALUE);
	lcd_cmd_write(LCD_TPSR,LCD_TPSR_VALUE);
	lcd_cmd_write(LCD_TPXR,LCD_TPXR_VALUE);
	lcd_cmd_write(LCD_TPYR,LCD_TPYR_VALUE);
	lcd_cmd_write(LCD_TPZR,LCD_TPZR_VALUE);

	//液晶亮度控制
	lcd_cmd_write(LCD_LCCR,LCD_LCCR_VALUE);
	lcd_cmd_write(LCD_PNTR,LCD_PNTR_VALUE);
	//字体控制寄存器
	//简体中文大字库，启动字体转换功能
	lcd_cmd_write(LCD_FNCR,LCD_FNCR_VALUE);//FNCR，Font Control Register
	lcd_cmd_write(LCD_FVHT,LCD_FVHT_VALUE);//FVHT字体不放大
	lcd_clear();
	delay_us(10);
}


void lcd_clear(void)
{
/*
	液晶清屏
*/
	unsigned char reg;
	lcd_cmd_write(0xe0,0x00);
	reg = lcd_cmd_read(0xf0);
	reg &=0xf7;
	reg |=0x08;
	lcd_cmd_write(0xf0,reg);
	delay_us(300);
}


void lcd_cursor_xy(unsigned int xn,unsigned int yn)
{
/*
	光标位置设定
参数：
	xn：x坐标，取值范围为0~40
	yn：y坐标，取值范围为0~240
*/
	lcd_cmd_write(LCD_CPXR,xn);
	lcd_cmd_write(LCD_CPYR,yn);
}

void lcd_print_str_zoom(unsigned char *str, unsigned char height, unsigned char width)
{
/*
	显示字符串
参数：
	str：字符串起始指针
	height：垂直方向放大倍数，0~3分别对应为不放大、放大1、2、3倍
	width：水平方向放大倍数，0~3分别对应为不放大、放大1、2、3倍
说明：
	放大显示是根据写进村器FVHT实现的；
	该寄存器7-6两位设置水平方向放大倍数
	该寄存器5-4两位设置数值方向放大倍数
*/
	width<<=4;
	width+=height<<6;
	width+=0x0f;
	lcd_cmd_write(LCD_WLCR,0xcd);
	lcd_cmd_write(LCD_FVHT,width); //放大一倍
	while(*str != '\0')	lcd_data_write(*str++);
}


