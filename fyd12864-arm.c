/*
		FYD12864-0402Bsm液晶显示驱动库

作者：冯建波
日期：2009/08/20
修订：v0.3
开发环境：
		WindowsXP+TKStudio
适用环境：
		带字库的12864液晶（控制芯片ST7920）
说明：
		适用于带汉字字库的12864
版本：
	2008/08/12	V0.1
		++字符显示的基本功能
	2008/11/03	V0.2
		xx修复函数参数校验
		++图形显示、反显、滚动等扩展功能
	2009/08/20	V0.3
		++移植到ARM LM1138

*/

/*********************************************宏定义*********************************************/
//文件包含
#include  <hw_types.h>
#include  <hw_memmap.h>
#include  <sysctl.h>
#include  <gpio.h>
#include "fyd12864-arm.h"

//端口定义
#define  LCD_DATA	GPIO_PORTA_BASE     //A口8位接液晶DB0到DB7，传输数据和指令
#define  LCD_BUSY	GPIO_PIN_0
#define  LCD_CTRL	GPIO_PORTB_BASE     //B口用到PB0~PB3,作控制用
#define  LCD_RS		GPIO_PIN_0
#define  LCD_RW		GPIO_PIN_1
#define  LCD_EA		GPIO_PIN_2

//基本指令定义
#define LCD_CMD_CLR 0x01		//清屏

//扩展指令定义
#define LCD_CMD_ROLL_ON 0x03     //开卷动
#define LCD_CMD_ROLL_OFF 0x02    //关卷动

#define LCD_PIC_ENABLE 0x36
#define LCD_PIC_DISABLE 0x30

/*	函数实现	*/
#ifndef delay_us
void delay_us(long iTime)
{
    iTime = SysCtlClockGet() * iTime / 2000000;             //  根据系统时钟速率确定延时
    while (--iTime != 0);
}
#endif

void lcd_wait_busy(void)
{/*		液晶等忙函数

说明：
	RS置零、RW置位，EA下降沿之后读忙位，BF为0则空闲
*/
	unsigned char busy;
	GPIOPinWrite(LCD_DATA , 0xff , 0xff);

	GPIOPinWrite(LCD_CTRL,LCD_RS,0);
	GPIOPinWrite(LCD_CTRL,LCD_RW,1<<1);
	do{
		GPIOPinWrite(LCD_CTRL,LCD_EA,0x01<<2);
		GPIOPinTypeGPIOInput(LCD_DATA , 0xff);           //  设置PA输入类型
		delay_us(1);
		busy=GPIOPinRead(LCD_DATA , 0xff);
		GPIOPinTypeGPIOOutput(LCD_DATA ,0xff);
		GPIOPinWrite(LCD_CTRL,LCD_EA,0x00);
               
	}while(busy>0x7f);

	delay_us(10);
}


void lcd_write_cmd(unsigned char cmd)
{/*		液晶写指令函数

参数： 指令代码
调用： lcd_wait_busy()

说明：
		RS、RW置零，EA下降沿写入指令
*/
		lcd_wait_busy();
		GPIOPinWrite(LCD_CTRL,LCD_RS,0);
		GPIOPinWrite(LCD_CTRL,LCD_RW,0x00);
		GPIOPinWrite(LCD_CTRL,LCD_EA,0x01<<2);
		GPIOPinWrite(LCD_DATA,0xff,cmd);
		GPIOPinWrite(LCD_CTRL,LCD_EA,0x00);
		GPIOPinWrite(LCD_DATA,0xff,0xff);

}

void lcd_write_data(unsigned char db)
{/*		液晶写数据函数

参数： 数据代码
返回： 空
调用： lcd_wait_busy()

说明：
		RS置位、RW置零，EA下降沿写入指令
*/
		lcd_wait_busy();
		GPIOPinWrite(LCD_CTRL,LCD_RS,0x01);
		GPIOPinWrite(LCD_CTRL,LCD_RW,0x00);
		GPIOPinWrite(LCD_CTRL,LCD_EA,0x01<<2);
		GPIOPinWrite(LCD_DATA,0xff,db);
		GPIOPinWrite(LCD_CTRL,LCD_EA,0x00);
		GPIOPinWrite(LCD_DATA,0xff,0xff);
}


void lcd_clr(void)
{/*		液晶清屏

参数： 空
返回： 空
调用： lcd_write_cmd()

说明：
		写指令0x01
*/
	lcd_write_cmd(LCD_CMD_CLR);
}


void lcd_expand_switch(unsigned char action)
{/*		液晶开关扩展指令

调用：lcd_write_cmd()
*/
	action=action?0x34:0x30;
	lcd_write_cmd(action);
}


void lcd_pos_set(unsigned char x,unsigned char y)
{/*		液晶写入位置设置函数

参数： x（0~7）、y（0~3）坐标
返回： 空
调用： lcd_write_cmd()

*/
	ASSERT(x>7 || y>3);	//参数有效性检查
	
	//生成并写入DDRAM地址（第2、3、4行地址分别加10、8、18，省点内存^_^）
	switch(y){//确定行起始地址
		case 0x00: y=0x80; break;
		case 0x01: y=0x90; break;
		case 0x02: y=0x88; break;
		case 0x03: y=0x98; break;
    }
	lcd_write_cmd(x+y);

}


void lcd_init(void)
{
/*
	液晶初始化函数

参数： 空
返回： 空
调用： delay100us()、lcd_write_cmd()

说明：
	重启ST7920、复位，设置功能，清屏
*/

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);		//注意使用I/O口前一定要使能
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPinTypeGPIOOutput(LCD_CTRL , GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);		   //  设置PB0~PB3为输出类型
	GPIOPinTypeGPIOOutput(LCD_DATA , 0xff);
	//功能设置
	lcd_expand_switch(0); //8BIT控制界面，基本指令集
	delay_us(10);
	lcd_write_cmd(0x0c);	//开显示、关光标，不闪烁
	delay_us(10);
	lcd_write_cmd(LCD_CMD_CLR); //清除屏幕显示，将DDRAM的地址计数器清零
	delay_us(10);
	lcd_write_cmd(0x06); //DDRAM的地址计数器（AC）加1
	//lcd_clr_pic();
	delay_us(10);
}


void  lcd_print_strxy(unsigned char *str,unsigned char x, unsigned char y) 
{/*		液晶定起点写字串函数

参数： 字串、xy坐标
返回： 空
调用： lcd_write_cmd()、lcd_write_data()、lcd_pos_set()
*/
    unsigned char i=0;
	lcd_pos_set(x,y);
	
    while ((str[i])!='\0'){
		lcd_write_data(str[i++]);
		if(i==16)	break;	//到行末就不写了
	}
}


void lcd_print_numxy(long int number, unsigned char x, unsigned char y)
{/*	定终点打印数值

参数：
	显示的数值、行号、列号
调用：
	lcd_write(1,)、lcd_position()
*/
	unsigned char str[11]={'\0'};
	unsigned char i=0;

	do{
		str[i]=number%10+48;
		number/=10;
		i++;
	}while(number);

	lcd_pos_set(x,y);
    while(i)      lcd_write_data(str[--i]);
}


void lcd_print_picxy(unsigned short dat, unsigned char x, unsigned char y)
{/*    	定终点打印图像
参数：
	dat:	显示的点阵，十六位，数据为该行要显示的点阵
	x、y:x从0~7，T=y从0～63
调用：
	lcd_expand_switch()、lcd_write(1,)、lcd_position()
*/
	//参数检查
	ASSERT(x>7 || y>63);
	lcd_expand_switch(1);
	lcd_write_cmd(LCD_PIC_ENABLE);
	
	//将行-列坐标转换为LCD的垂直-水平坐标
	if(y>31){
		x+=8;
		y-=32;    
	}
	
	//先后写入垂直、水平地址：0x80+addr
	lcd_write_cmd(0x80+y);
	lcd_write_cmd(0x80+x);
	
	//先后写入两个字节的数据：0x80+addr;
	lcd_write_data(dat);
	lcd_write_data(dat>>8);
	
	lcd_write_cmd(LCD_PIC_DISABLE);
	lcd_expand_switch(0);
}


void  lcd_negative(unsigned char action, unsigned char x, unsigned char y) 
{/*		液晶定点反白函数

参数： 
	action：有效时反白，无效时取消反白
	x、y：XY坐标
调用：
	lcd_print_picxy()

*/
	unsigned char i=0;
	unsigned short dat;
	ASSERT(x>7 || y>3);
	
	dat=action?0xffff:0x0000;
	for(i=0; i<16; i++)	lcd_print_picxy(dat,x,i+y*16);
}


void lcd_clr_pic(void)
{
    /*
		    液晶清屏

    参数： 空
    返回： 空
    调用： lcd_write_cmd()

    说明：
		    写指令0x01
*/	
	unsigned char x=0,y=0;

	lcd_expand_switch(1);
	lcd_write_cmd(LCD_PIC_ENABLE);
	for(y=0; y<4; y++){
		lcd_write_cmd(0x80+y);
		lcd_write_cmd(0x80);
		for (x=0; x<16; x++){
			lcd_write_data(0x00);
			lcd_write_data(0x00);
		}
	}
	lcd_write_cmd(LCD_PIC_DISABLE);
	lcd_expand_switch(0);
}


void lcd_roll(unsigned char y)
{/*		液晶卷动

参数： 卷动行编号(1~4)
调用： lcd_expand_switch()、lcd_write_cmd()
*/
	lcd_expand_switch(1);
	lcd_write_cmd(LCD_CMD_ROLL_ON);
	lcd_write_cmd(0x40 | y);
	lcd_expand_switch(0);
}

void lcd_roll_disable(void)
{/*		液晶关卷动

调用：lcd_expand_switch()、lcd_write_cmd()
*/
	lcd_expand_switch(1);
	lcd_write_cmd(LCD_CMD_ROLL_OFF);
	lcd_expand_switch(0);
}



