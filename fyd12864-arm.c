/*
		FYD12864-0402BsmҺ����ʾ������

���ߣ��뽨��
���ڣ�2009/08/20
�޶���v0.3
����������
		WindowsXP+TKStudio
���û�����
		���ֿ��12864Һ��������оƬST7920��
˵����
		�����ڴ������ֿ��12864
�汾��
	2008/08/12	V0.1
		++�ַ���ʾ�Ļ�������
	2008/11/03	V0.2
		xx�޸���������У��
		++ͼ����ʾ�����ԡ���������չ����
	2009/08/20	V0.3
		++��ֲ��ARM LM1138

*/

/*********************************************�궨��*********************************************/
//�ļ�����
#include  <hw_types.h>
#include  <hw_memmap.h>
#include  <sysctl.h>
#include  <gpio.h>
#include "fyd12864-arm.h"

//�˿ڶ���
#define  LCD_DATA	GPIO_PORTA_BASE     //A��8λ��Һ��DB0��DB7���������ݺ�ָ��
#define  LCD_BUSY	GPIO_PIN_0
#define  LCD_CTRL	GPIO_PORTB_BASE     //B���õ�PB0~PB3,��������
#define  LCD_RS		GPIO_PIN_0
#define  LCD_RW		GPIO_PIN_1
#define  LCD_EA		GPIO_PIN_2

//����ָ���
#define LCD_CMD_CLR 0x01		//����

//��չָ���
#define LCD_CMD_ROLL_ON 0x03     //����
#define LCD_CMD_ROLL_OFF 0x02    //�ؾ�

#define LCD_PIC_ENABLE 0x36
#define LCD_PIC_DISABLE 0x30

/*	����ʵ��	*/
#ifndef delay_us
void delay_us(long iTime)
{
    iTime = SysCtlClockGet() * iTime / 2000000;             //  ����ϵͳʱ������ȷ����ʱ
    while (--iTime != 0);
}
#endif

void lcd_wait_busy(void)
{/*		Һ����æ����

˵����
	RS���㡢RW��λ��EA�½���֮���æλ��BFΪ0�����
*/
	unsigned char busy;
	GPIOPinWrite(LCD_DATA , 0xff , 0xff);

	GPIOPinWrite(LCD_CTRL,LCD_RS,0);
	GPIOPinWrite(LCD_CTRL,LCD_RW,1<<1);
	do{
		GPIOPinWrite(LCD_CTRL,LCD_EA,0x01<<2);
		GPIOPinTypeGPIOInput(LCD_DATA , 0xff);           //  ����PA��������
		delay_us(1);
		busy=GPIOPinRead(LCD_DATA , 0xff);
		GPIOPinTypeGPIOOutput(LCD_DATA ,0xff);
		GPIOPinWrite(LCD_CTRL,LCD_EA,0x00);
               
	}while(busy>0x7f);

	delay_us(10);
}


void lcd_write_cmd(unsigned char cmd)
{/*		Һ��дָ���

������ ָ�����
���ã� lcd_wait_busy()

˵����
		RS��RW���㣬EA�½���д��ָ��
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
{/*		Һ��д���ݺ���

������ ���ݴ���
���أ� ��
���ã� lcd_wait_busy()

˵����
		RS��λ��RW���㣬EA�½���д��ָ��
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
{/*		Һ������

������ ��
���أ� ��
���ã� lcd_write_cmd()

˵����
		дָ��0x01
*/
	lcd_write_cmd(LCD_CMD_CLR);
}


void lcd_expand_switch(unsigned char action)
{/*		Һ��������չָ��

���ã�lcd_write_cmd()
*/
	action=action?0x34:0x30;
	lcd_write_cmd(action);
}


void lcd_pos_set(unsigned char x,unsigned char y)
{/*		Һ��д��λ�����ú���

������ x��0~7����y��0~3������
���أ� ��
���ã� lcd_write_cmd()

*/
	ASSERT(x>7 || y>3);	//������Ч�Լ��
	
	//���ɲ�д��DDRAM��ַ����2��3��4�е�ַ�ֱ��10��8��18��ʡ���ڴ�^_^��
	switch(y){//ȷ������ʼ��ַ
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
	Һ����ʼ������

������ ��
���أ� ��
���ã� delay100us()��lcd_write_cmd()

˵����
	����ST7920����λ�����ù��ܣ�����
*/

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);		//ע��ʹ��I/O��ǰһ��Ҫʹ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPinTypeGPIOOutput(LCD_CTRL , GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);		   //  ����PB0~PB3Ϊ�������
	GPIOPinTypeGPIOOutput(LCD_DATA , 0xff);
	//��������
	lcd_expand_switch(0); //8BIT���ƽ��棬����ָ�
	delay_us(10);
	lcd_write_cmd(0x0c);	//����ʾ���ع�꣬����˸
	delay_us(10);
	lcd_write_cmd(LCD_CMD_CLR); //�����Ļ��ʾ����DDRAM�ĵ�ַ����������
	delay_us(10);
	lcd_write_cmd(0x06); //DDRAM�ĵ�ַ��������AC����1
	//lcd_clr_pic();
	delay_us(10);
}


void  lcd_print_strxy(unsigned char *str,unsigned char x, unsigned char y) 
{/*		Һ�������д�ִ�����

������ �ִ���xy����
���أ� ��
���ã� lcd_write_cmd()��lcd_write_data()��lcd_pos_set()
*/
    unsigned char i=0;
	lcd_pos_set(x,y);
	
    while ((str[i])!='\0'){
		lcd_write_data(str[i++]);
		if(i==16)	break;	//����ĩ�Ͳ�д��
	}
}


void lcd_print_numxy(long int number, unsigned char x, unsigned char y)
{/*	���յ��ӡ��ֵ

������
	��ʾ����ֵ���кš��к�
���ã�
	lcd_write(1,)��lcd_position()
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
{/*    	���յ��ӡͼ��
������
	dat:	��ʾ�ĵ���ʮ��λ������Ϊ����Ҫ��ʾ�ĵ���
	x��y:x��0~7��T=y��0��63
���ã�
	lcd_expand_switch()��lcd_write(1,)��lcd_position()
*/
	//�������
	ASSERT(x>7 || y>63);
	lcd_expand_switch(1);
	lcd_write_cmd(LCD_PIC_ENABLE);
	
	//����-������ת��ΪLCD�Ĵ�ֱ-ˮƽ����
	if(y>31){
		x+=8;
		y-=32;    
	}
	
	//�Ⱥ�д�봹ֱ��ˮƽ��ַ��0x80+addr
	lcd_write_cmd(0x80+y);
	lcd_write_cmd(0x80+x);
	
	//�Ⱥ�д�������ֽڵ����ݣ�0x80+addr;
	lcd_write_data(dat);
	lcd_write_data(dat>>8);
	
	lcd_write_cmd(LCD_PIC_DISABLE);
	lcd_expand_switch(0);
}


void  lcd_negative(unsigned char action, unsigned char x, unsigned char y) 
{/*		Һ�����㷴�׺���

������ 
	action����Чʱ���ף���Чʱȡ������
	x��y��XY����
���ã�
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
		    Һ������

    ������ ��
    ���أ� ��
    ���ã� lcd_write_cmd()

    ˵����
		    дָ��0x01
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
{/*		Һ����

������ ���б��(1~4)
���ã� lcd_expand_switch()��lcd_write_cmd()
*/
	lcd_expand_switch(1);
	lcd_write_cmd(LCD_CMD_ROLL_ON);
	lcd_write_cmd(0x40 | y);
	lcd_expand_switch(0);
}

void lcd_roll_disable(void)
{/*		Һ���ؾ�

���ã�lcd_expand_switch()��lcd_write_cmd()
*/
	lcd_expand_switch(1);
	lcd_write_cmd(LCD_CMD_ROLL_OFF);
	lcd_expand_switch(0);
}



