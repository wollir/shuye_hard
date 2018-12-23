#include "wer_task.h"
#include "delay.h"
#include "UART_conf.h"
#include "main.h"
#include "oled.h"
#include "bmp.h"
//void led_B2_init(void)
//{
//  GPIO_InitTypeDef  GPIO_InitStructure;

//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//??GPIOF??

//  //GPIOF9,F10?????
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//LED0?LED1??IO?
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//??????
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//???GPIO
//	
//	GPIO_SetBits(GPIOB,GPIO_Pin_2);//GPIOF9,F10???,??
//}
void send_data(unsigned char* data,int datasize)  // 要以什么格式发送，先看上位机以什么格式收。
{
	int i = 0;
	for(; i<datasize-4;i++)
		wer_send(data[i]);
		//delay_ms(500);
#if wireless			
		  wer_send(0x01);wer_send(0x00);wer_send(CHANNAL);// 主机地址
#endif
	for(; i<datasize;i++){
		wer_send(data[i]);
	}
}
void send_data_echo(unsigned char* data,int datasize)  // 要以什么格式发送，先看上位机以什么格式收。
{
	int i;
	for(i = 0; i<datasize;i++)
		wer_send(data[i]);	
}
void send_TempHumi(sht_data data)
{
	wer_send(data.tempL);
	wer_send(data.tempH);
	wer_send(data.humiL);
	wer_send(data.humiH);
}
void SortFrom3648(u8* data,u8* target_data,int target_datasize)
{
		int i;
    short frequency_demultiplication =3648/target_datasize;  // 3648/60=60.8 总共
    for(i = 24 ;i < 3648-24;i+=frequency_demultiplication)
    {
        target_data[i/frequency_demultiplication]= data[i];
    }
}
void recongnitionData(received_res* recon_data,volatile uint8_t receive[])
{
	recon_data->temp = ((receive[2]<<8) + receive[3]);
	recon_data->hum = ((receive[4]<<8) + receive[5]);
	recon_data->height = receive[6];
	recon_data->height_diff = receive[7];
	recon_data->alart = receive[8];
	recon_data->id = (receive[9]<<8) + receive[10];
}
void display_oled(received_res* recon_data)
{
	// 温度显示
	u16 wen = recon_data->temp;
	OLED_ShowNum(0,3,wen/10,2,16);
	OLED_ShowString(16,3,".",16);
	OLED_ShowNum(24,3,wen%10,1,16);
	//湿度
	u16 shi = recon_data->hum;
	OLED_ShowNum(90,3,shi/10,2,16);
	OLED_ShowString(106,3,".",16);
	OLED_ShowNum(112,3,shi%10,1,16);
	
	//高度
	u16 height = recon_data->height;
	OLED_ShowNum(64,6,height/10,2,16);
	OLED_ShowString(80,6,".",16);
	OLED_ShowNum(88,6,height%10,1,16);
}
void oled_AllInit(void)
{
		OLED_Init();			//初始化OLED  
		OLED_Clear(); 
		u8 add = 9;
		OLED_ShowCHinese(0+add,0,0);//
		OLED_ShowCHinese(18+add,0,1);//
		OLED_ShowCHinese(36+add,0,2);//
		OLED_ShowCHinese(54+add,0,3);//
		OLED_ShowCHinese(72+add,0,4);//
		OLED_ShowCHinese(90+add,0,5);//
		OLED_ShowCHinese(36,3,12); //摄氏度
		OLED_ShowString(120,3,"%",16);
	OLED_ShowString(0,6,"Height:     mm",16);
}
void oled_test1(void)
{
	u8 t;
		OLED_Init();			//初始化OLED  
		OLED_Clear()  	; 
	
		t=' ';
		OLED_ShowCHinese(0,0,0);//中
				OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
	while(1) 
	{		
		OLED_Clear();
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
		OLED_ShowString(6,3,"0.96' OLED TEST",16);
		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 //	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:",16);  
		OLED_ShowString(63,6,"CODE:",16);  
		OLED_ShowChar(48,6,t,16);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值 	
			delay_ms(3000);
		//delay_ms(8000);

		//			delay_ms(8000);
		//delay_ms(8000);
		//delay_ms(8000);
		OLED_DrawBMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		delay_ms(3000);
		//			delay_ms(8000);
		//delay_ms(8000);
		//delay_ms(8000);
		//OLED_DrawBMP(0,0,128,8,BMP1);
		//delay_ms(8000);
		//			delay_ms(8000);
		//delay_ms(8000);
		//delay_ms(3000);
	}
}
