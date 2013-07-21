#include "ra8803.h"


/*	�Ĵ�����ַ����	*/

//Һ��ȫ�ֿ�����
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
	��ʱ΢����
������
	n:΢����
*/
	while(n--);
}
#endif

void lcd_cmd_write (unsigned char addr , unsigned char db){
/*
	дָ��Ĵ���
������
	cmd��ָ��Ĵ�����ַ
	cmd_data��Ҫд���ֵ

˵����
	дָ����̣���д��Ĵ�����ַ����д��Ĵ�������
	RD=L�����ź���Ч��	RS=L��ָ��Ĵ�����	WR��д��
	
*/
	LCD_DATA=addr;

	LCD_CS=0;
	LCD_RD=1;	LCD_RS=0;
	LCD_WR=0;	LCD_WR=1;	//WR������д��
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
	��ָ��Ĵ���
������
	addr��ָ��Ĵ�����ַ
���أ�
	��Ӧ�ļĴ���ֵ
˵����
	��ָ��Ĵ������̣�
		��д��Ĵ�����ַ��	RD=L�����ź���Ч��	RS=L��ָ��Ĵ�����	WR��д��
		�ٶ����Ĵ������ݣ�	WR=L��д�ź���Ч��	RS=L��ָ��Ĵ�����	RD��д��
*/
	unsigned char db;
	//д��ַ
	LCD_DATA=addr;
	LCD_CS=0;
	LCD_RD=1;	LCD_RS=0;
	LCD_WR=0;
	LCD_WR=1;	LCD_RS=1;	LCD_CS=1;

	//�����ݲ�����
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
	д��ʾ����
������
	db��Ҫ��ʾ������
˵����
	RD=L�����ź���Ч��	RS=H�����ݼĴ�����	WR��д��
*/
	LCD_DATA=db;
	LCD_CS=0;	LCD_RD=1;	LCD_RS=1;
	LCD_WR=0;	LCD_WR=1;	//WR������д��
	LCD_RS=0;	LCD_CS=1;
}


unsigned char lcd_data_read(void)
{
/*
	����ʾ����
������
	db��Ҫ��ʾ������
˵����
	RD=L�����ź���Ч��	RS=H�����ݼĴ�����	WR��д��
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
	Һ����ʼ��
*/
	lcd_cmd_write(LCD_WLCR,LCD_WLCR_VALUE);//WLCR ����ģʽ������ģʽ����Ļ����������˸��������
	/*
		��������
	���񣨵���λ������λΪ00~11�ֱ��Ӧ3��4��8��12MHz
	*/
	lcd_cmd_write(LCD_MISC,LCD_MISC_VALUE);
	lcd_cmd_write(LCD_ASPR,LCD_APSR_VALUE);//APSR
	lcd_cmd_write(LCD_ADSR,LCD_ADSR_VALUE);//ADSR ƽ�Ʒ��򶯿���
	lcd_cmd_write(LCD_WCCR,LCD_WCCR_VALUE);//WCCR ������ʾ�����Ӵ�
	lcd_cmd_write(LCD_MAMR,LCD_MAMR_VALUE);//ͼ������

	//��ʾ������ڷ�Χ��ʼ��
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

	//���������ơ�״̬�Ĵ���
	lcd_cmd_write(LCD_TPCR,LCD_TPCR_VALUE);
	lcd_cmd_write(LCD_TPSR,LCD_TPSR_VALUE);
	lcd_cmd_write(LCD_TPXR,LCD_TPXR_VALUE);
	lcd_cmd_write(LCD_TPYR,LCD_TPYR_VALUE);
	lcd_cmd_write(LCD_TPZR,LCD_TPZR_VALUE);

	//Һ�����ȿ���
	lcd_cmd_write(LCD_LCCR,LCD_LCCR_VALUE);
	lcd_cmd_write(LCD_PNTR,LCD_PNTR_VALUE);
	//������ƼĴ���
	//�������Ĵ��ֿ⣬��������ת������
	lcd_cmd_write(LCD_FNCR,LCD_FNCR_VALUE);//FNCR��Font Control Register
	lcd_cmd_write(LCD_FVHT,LCD_FVHT_VALUE);//FVHT���岻�Ŵ�
	lcd_clear();
	delay_us(10);
}


void lcd_clear(void)
{
/*
	Һ������
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
	���λ���趨
������
	xn��x���꣬ȡֵ��ΧΪ0~40
	yn��y���꣬ȡֵ��ΧΪ0~240
*/
	lcd_cmd_write(LCD_CPXR,xn);
	lcd_cmd_write(LCD_CPYR,yn);
}

void lcd_print_str_zoom(unsigned char *str, unsigned char height, unsigned char width)
{
/*
	��ʾ�ַ���
������
	str���ַ�����ʼָ��
	height����ֱ����Ŵ�����0~3�ֱ��ӦΪ���Ŵ󡢷Ŵ�1��2��3��
	width��ˮƽ����Ŵ�����0~3�ֱ��ӦΪ���Ŵ󡢷Ŵ�1��2��3��
˵����
	�Ŵ���ʾ�Ǹ���д������FVHTʵ�ֵģ�
	�üĴ���7-6��λ����ˮƽ����Ŵ���
	�üĴ���5-4��λ������ֵ����Ŵ���
*/
	width<<=4;
	width+=height<<6;
	width+=0x0f;
	lcd_cmd_write(LCD_WLCR,0xcd);
	lcd_cmd_write(LCD_FVHT,width); //�Ŵ�һ��
	while(*str != '\0')	lcd_data_write(*str++);
}


