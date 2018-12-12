#ifndef __WERTASK
#define __WERTASK
#include "sht30.h"
#include "stm32f4xx.h"

#include "main.h"

void send_data(unsigned char* data,int datasize);
void send_data_echo(unsigned char* data,int datasize);
void led_B2_init(void);
void SortFrom3648(u8* data,u8* target_data,int target_datasize);
void send_TempHumi(sht_data data);

#define	ledb2(a)      {if(a) GPIO_SetBits(GPIOB,GPIO_Pin_2); else	GPIO_ResetBits(GPIOB,GPIO_Pin_2);}//

#define	ledA4(a)      {if(a) GPIO_SetBits(GPIOA,GPIO_Pin_4); else	GPIO_ResetBits(GPIOA,GPIO_Pin_4);}//
#define	ledA5(a)      {if(a) GPIO_SetBits(GPIOA,GPIO_Pin_5); else	GPIO_ResetBits(GPIOA,GPIO_Pin_5);}//
#define	ledA6(a)      {if(a) GPIO_SetBits(GPIOA,GPIO_Pin_6); else	GPIO_ResetBits(GPIOA,GPIO_Pin_6);}//
#define	ledA7(a)      {if(a) GPIO_SetBits(GPIOA,GPIO_Pin_7); else	GPIO_ResetBits(GPIOA,GPIO_Pin_7);}//
#define	ledF9(a)      {if(a) GPIO_SetBits(GPIOF,GPIO_Pin_9); else	GPIO_ResetBits(GPIOF,GPIO_Pin_9);}//
#define	ledF10(a)     {if(a) GPIO_SetBits(GPIOF,GPIO_Pin_10); else	GPIO_ResetBits(GPIOF,GPIO_Pin_10);}//

#if MYBOARD
	#define leda(a)  ledA6(a)
	#define ledb(a)  ledA5(a)
	#define beep(a)  ledA7(a)
	#define	back_led(a) ledA4(a)
#else
#if (ID == 1)
	#define leda(a)  ledA6(a)
	#define beep(a)  ledA7(a)
	#define	back_led(a) {if(!a) GPIO_SetBits(GPIOA,GPIO_Pin_4); else	GPIO_ResetBits(GPIOA,GPIO_Pin_4);}//
#elseif (ID == 3)
	#define leda(a)  ledF9(a)
	#define beep(a)  ledF10(a)
	#define	back_led(a) {if(!a) GPIO_SetBits(GPIOF,GPIO_Pin_8); else	GPIO_ResetBits(GPIOF,GPIO_Pin_8);}//

#endif
#endif
#endif
