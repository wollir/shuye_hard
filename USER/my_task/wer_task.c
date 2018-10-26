#include "wer_task.h"
#include "delay.h"
#include "UART_conf.h"
#include "main.h"
void led_B2_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//??GPIOF??

  //GPIOF9,F10?????
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//LED0?LED1??IO?
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//??????
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
  GPIO_Init(GPIOB, &GPIO_InitStructure);//???GPIO
	
	GPIO_SetBits(GPIOB,GPIO_Pin_2);//GPIOF9,F10???,??
}
void send_data(unsigned char* data,int datasize)  // 要以什么格式发送，先看上位机以什么格式收。
{
	int i = 0;
	for(; i<datasize-4;i++)
		wer_send(data[i]);
		//delay_ms(500);
#ifdef wireless			
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
    short frequency_demultiplication =3648/target_datasize;  // 3648/60=60.8
    for(i = 1 ;i < 3648;i+=frequency_demultiplication)
    {
        target_data[i/frequency_demultiplication]= data[i];
    }
}
