/*
        FYD12864-0402BsmҺ����ʾ������

���ߣ��뽨��
���ڣ�2008-11-03
�޶���v0.2
����������
        WindowsXP+TKStudio
���û�����
        ���ֿ��12864Һ��������оƬST7920��

˵����
        �����ڴ������ֿ��12864

*/

/*********************************************�궨��*********************************************/
//�ļ�����
#include "common.h"
#include "fyd12864.h"

//�˿ڶ���
#define LCD_DB_BUS	P0
#define LCD_BUSY		P0_7
#define LCD_RS		P2_0
#define LCD_RW		P2_1
#define LCD_EA		P2_2
#define LCD_PSB		P3_3

//����ָ���
#define LCD_CMD_CLR 0x01        //����
//��չָ���

#define LCD_CMD_ROLL_ON 0x03     //����
#define LCD_CMD_ROLL_OFF 0x02    //�ؾ�

#define LCD_PIC_ENABLE 0x36
#define LCD_PIC_DISABLE 0x30

/*	����ʵ��	*/

void lcd_wait_busy(void)
{/*        Һ����æ����

˵����
        RS���㡢RW��λ��EA�½���֮���æλ��BFΪ0�����

	unsigned char busy;
	LCD_DB_BUS=0xff;
	LCD_RS = 0;
    LCD_RW = 1;
	LCD_EA=1;
	while(LCD_BUSY);
	LCD_EA=0;
*/
	delay_10us(1);
}


void lcd_write_cmd(unsigned char cmd)
{/*        Һ��дָ���

������ ָ�����
���ã� lcd_wait_busy()

˵����
        RS��RW���㣬EA�½���д��ָ��
*/
        lcd_wait_busy();
        LCD_RS=0;
        LCD_RW=0;
        LCD_EA=1;
        LCD_DB_BUS =cmd;
        LCD_EA=0;
		_nop_();
		_nop_();
        LCD_DB_BUS =0xff;

}

void lcd_write_data(unsigned char db)
{/*        Һ��д���ݺ���

������ ���ݴ���
���أ� ��
���ã� lcd_wait_busy()

˵����
        RS��λ��RW���㣬EA�½���д��ָ��
*/
        lcd_wait_busy();
        LCD_RS=1;
        LCD_RW=0;
        LCD_EA=1;
        LCD_DB_BUS =db;
        LCD_EA=0;
		_nop_();
		_nop_();
        LCD_DB_BUS =0xff;
}


void lcd_clr(void)
{/*        Һ������

������ ��
���أ� ��
���ã� lcd_write_cmd()

˵����
        дָ��0x01
*/
	lcd_write_cmd(LCD_CMD_CLR);
}


void lcd_expand_switch(unsigned char action)
{/*        Һ��������չָ��

���ã�lcd_write_cmd()
*/
	action=action?0x34:0x30;
	lcd_write_cmd(action);
}


void lcd_pos_set(unsigned char x,unsigned char y)
{/*        Һ��д��λ�����ú���

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

        //��������
        lcd_expand_switch(0); //8BIT���ƽ��棬����ָ�
		delay_10us(1);
		lcd_write_cmd(0x0c);	//����ʾ���ع�꣬����˸
		delay_10us(1);
        lcd_write_cmd(LCD_CMD_CLR); //�����Ļ��ʾ����DDRAM�ĵ�ַ����������
        delay_10us(1);
		lcd_write_cmd(0x06); //DDRAM�ĵ�ַ��������AC����1
		lcd_clr_pic();
		delay_10us(1);
}


void  lcd_print_strxy(unsigned char *str,unsigned char x, unsigned char y) 
{/*        Һ�������д�ִ�����

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


void lcd_print_picxy(unsigned int dat, unsigned char x, unsigned char y)
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
{/*        Һ�����㷴�׺���

������ 
	action����Чʱ���ף���Чʱȡ������
	x��y��XY����
���ã�
	lcd_print_picxy()

*/
	unsigned char i=0;
	unsigned int dat;
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
    for (y=0; y<4; y++)
    	for (x=0; x<8; x++)
    		lcd_negative(0,x,y);

}


void lcd_roll(unsigned char y)
{/*        Һ����

������ ���б��(1~4)
���ã� lcd_expand_switch()��lcd_write_cmd()
*/
	lcd_expand_switch(1);
	lcd_write_cmd(LCD_CMD_ROLL_ON);
	lcd_write_cmd(0x40 | y);
	lcd_expand_switch(0);
}

void lcd_roll_disable(void)
{/*        Һ���ؾ�

���ã�lcd_expand_switch()��lcd_write_cmd()
*/
	lcd_expand_switch(1);
	lcd_write_cmd(LCD_CMD_ROLL_OFF);
	lcd_expand_switch(0);
}



