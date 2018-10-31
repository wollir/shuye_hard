#ifndef __WERTASK
#define __WERTASK
#include"sht30.h"
#include "stm32f4xx.h"
#define wireless
void send_data(unsigned char* data,int datasize);
void send_data_echo(unsigned char* data,int datasize);
void led_B2_init(void);
void SortFrom3648(u8* data,u8* target_data,int target_datasize);
void send_TempHumi(sht_data data);

#define	ledB2(a)      {if(a) GPIO_SetBits(GPIOB,GPIO_Pin_2); else	GPIO_ResetBits(GPIOB,GPIO_Pin_2);}//

#define	ledF9(a)      {if(a) GPIO_SetBits(GPIOF,GPIO_Pin_9); else	GPIO_ResetBits(GPIOF,GPIO_Pin_9);}//
#define	ledF10(a)      {if(a) GPIO_SetBits(GPIOF,GPIO_Pin_10); else	GPIO_ResetBits(GPIOF,GPIO_Pin_10);}//

#define	back_ledF8(a) {if(a) GPIO_SetBits(GPIOF,GPIO_Pin_8); else	GPIO_ResetBits(GPIOF,GPIO_Pin_8);}//

#endif

