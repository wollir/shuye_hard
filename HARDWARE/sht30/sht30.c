#include "sht30.h"
#include "wer_delay.h"
#include "delay.h"
#include "UART_conf.h"
void start_mesure()
{
	IIC_Start();
	IIC_Send_Byte(0x88); //0x44地址+0（写）  0x45--0x4A  0x44--0x88
	IIC_Wait_Ack();
	IIC_Send_Byte(0x2c);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x0d);
	//IIC_Send_Byte(0x06);
	IIC_Wait_Ack();
	IIC_Stop();
	//Delay_Ms(5);
	delay_ms(5);
}
sht_data sht30_read()
{
	sht_data data;
	u8 temprature_L = 0,temprature_S = 0;
	u8 humidity_L = 0,humidity_S = 0;
	u8 crc; //没用
	crc = crc;
	IIC_Start();
	IIC_Send_Byte(0x89); //0x44地址+1（读）  0x44--0x89   0x45 -- 0x4b
	IIC_Wait_Ack();
	
	//IIC_Ack();
	//Delay_Ms(5);  //留出测量的时间
		delay_ms(5);
	temprature_S = IIC_Read_Byte(1);
	temprature_L = IIC_Read_Byte(1);
	crc = IIC_Read_Byte(1);
	humidity_S = IIC_Read_Byte(1);
	humidity_L = IIC_Read_Byte(1);
	crc = IIC_Read_Byte(0);
	IIC_Stop();
	
	data.tempH = temprature_S;
	data.tempL = temprature_L;
	
	data.humiH = humidity_S;
	data.humiL = humidity_L;
	return data;
}

//u8 aa()
//{
//	int real_temp = 0;
//	u8 real_hum;
//	sht_data test;
//	start_mesure();
//	//delay_us(100);
//	test = sht30_read();
//	return (u8)(real_temp);
//}
