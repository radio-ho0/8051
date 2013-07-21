#include "common.h"

void delay_s(unsigned char n)
{/*	延时数秒函数
参数：延时时间
*/
	unsigned char i,j;
	n*=10;
	while(n--){
		for(i=200;i>0;i--)
			for(j=200;j>0;j--)	_nop_();
	}
}


void  delay_ms(unsigned int n)
{/*        延时数毫秒函数
参数：时长（单位：毫秒）
*/
	unsigned char i;
	n*=2;
	while(n--)	for(i=0;i<200;i++)	_nop_();
 
}

void delay_10us(unsigned char n)
{/*        延时数十微秒函数
参数：时长（单位：毫秒）
*/
	n*=4;
	while(n--)	_nop_();
}
