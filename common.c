#include "common.h"

void delay_s(unsigned char n)
{/*	��ʱ���뺯��
��������ʱʱ��
*/
	unsigned char i,j;
	n*=10;
	while(n--){
		for(i=200;i>0;i--)
			for(j=200;j>0;j--)	_nop_();
	}
}


void  delay_ms(unsigned int n)
{/*        ��ʱ�����뺯��
������ʱ������λ�����룩
*/
	unsigned char i;
	n*=2;
	while(n--)	for(i=0;i<200;i++)	_nop_();
 
}

void delay_10us(unsigned char n)
{/*        ��ʱ��ʮ΢�뺯��
������ʱ������λ�����룩
*/
	n*=4;
	while(n--)	_nop_();
}
