#include "common.h"
#include "ds18b20.h"

unsigned char ds18b20_init(void)
{
/*
	初始化DS18D20函数

调用：delay_us()
*/
	unsigned char init_stats=0;
	DS18B20_DQ = 1;    //DS18B20_DQ复位
	delay_us(8); //稍做延时
	DS18B20_DQ = 0;    //单片机将DS18B20_DQ拉低
	delay_us(80); //精确延时 大于 480us
	DS18B20_DQ = 1;    //拉高总线
	delay_us(14);
	init_stats=DS18B20_DQ;      //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	delay_us(20);
	return init_stats;
}
//读一个字节
unsigned char ds18b20_read(void)
{
/*
	读DS18D20

返回：读到的数据，八位单字节
调用：delay_us()
*/
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--){
		DS18B20_DQ = 0; // 给脉冲信号
		dat>>=1;
		DS18B20_DQ = 1; // 给脉冲信号
		if(DS18B20_DQ)	dat|=0x80;
		delay_us(4);
	}
	return(dat);
}
//写一个字节
void ds18b20_write(unsigned char dat)
{
/*
	写DS18D20

参数：
	dat:写入的数据，八位单字节
调用：delay_us()
*/
	unsigned char i=0;
	for (i=8; i>0; i--){
		DS18B20_DQ = 0;
		DS18B20_DQ = dat&0x01;
		delay_us(5);
		DS18B20_DQ = 1;
		dat>>=1;
	}
	delay_us(4);
}

unsigned char ds18b20_get_temp(void)
{
/*
	读取温度

返回：
	温度，八位单字节
调用：delay_us()
*/
	unsigned char temp_h, temp_l;
	
	ds18b20_init();
	ds18b20_write(0xCC); // 跳过读序号列号的操作
	ds18b20_write(0x44); // 启动温度转换
	ds18b20_init();
	ds18b20_write(0xCC); //跳过读序号列号的操作
	ds18b20_write(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
	temp_l=ds18b20_read();   //读取温度值低位
	temp_h=ds18b20_read();   //读取温度值高位
	temp_l>>=4;
	temp_h&=0x07;
	temp_h<<=4;
	return((temp_l|temp_h)&0x7f);
}
