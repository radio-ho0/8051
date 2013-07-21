#ifndef DS18B20_H
#define DS18B20_H

/*	端口声明	*/
#define DS18B20_DQ	P2_0

/*	函数声明	*/
unsigned char ds18b20_init(void);
unsigned char ds18b20_read(void);
void ds18b20_write(unsigned char dat);
unsigned char ds18b20_get_temp(void);

#endif
