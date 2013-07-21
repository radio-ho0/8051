#include "common.h"
#include "ds18b20.h"

unsigned char ds18b20_init(void)
{
/*
	��ʼ��DS18D20����

���ã�delay_us()
*/
	unsigned char init_stats=0;
	DS18B20_DQ = 1;    //DS18B20_DQ��λ
	delay_us(8); //������ʱ
	DS18B20_DQ = 0;    //��Ƭ����DS18B20_DQ����
	delay_us(80); //��ȷ��ʱ ���� 480us
	DS18B20_DQ = 1;    //��������
	delay_us(14);
	init_stats=DS18B20_DQ;      //������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
	delay_us(20);
	return init_stats;
}
//��һ���ֽ�
unsigned char ds18b20_read(void)
{
/*
	��DS18D20

���أ����������ݣ���λ���ֽ�
���ã�delay_us()
*/
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--){
		DS18B20_DQ = 0; // �������ź�
		dat>>=1;
		DS18B20_DQ = 1; // �������ź�
		if(DS18B20_DQ)	dat|=0x80;
		delay_us(4);
	}
	return(dat);
}
//дһ���ֽ�
void ds18b20_write(unsigned char dat)
{
/*
	дDS18D20

������
	dat:д������ݣ���λ���ֽ�
���ã�delay_us()
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
	��ȡ�¶�

���أ�
	�¶ȣ���λ���ֽ�
���ã�delay_us()
*/
	unsigned char temp_h, temp_l;
	
	ds18b20_init();
	ds18b20_write(0xCC); // ����������кŵĲ���
	ds18b20_write(0x44); // �����¶�ת��
	ds18b20_init();
	ds18b20_write(0xCC); //����������кŵĲ���
	ds18b20_write(0xBE); //��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
	temp_l=ds18b20_read();   //��ȡ�¶�ֵ��λ
	temp_h=ds18b20_read();   //��ȡ�¶�ֵ��λ
	temp_l>>=4;
	temp_h&=0x07;
	temp_h<<=4;
	return((temp_l|temp_h)&0x7f);
}
