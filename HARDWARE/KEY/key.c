#include "key.h"
#include "delay.h" 
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//按键输入驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#if (ID == 1)

#else

//按键初始化函数
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	// PE4 按键0
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //KEY0 KEY1 KEY2对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4
 
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)
			return 1;
	}else if(KEY0==1)
		key_up=1; 	    
 	return 0;// 无按键按下
}

#endif

static void NVIC_Configuration(void)
{
	
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//????4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//?????1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//????2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//????????
  NVIC_Init(&NVIC_InitStructure);//??
}


/*
 * ???:EXTI_Config
 * ??  :??PA0,PA13,PA15?????,????????
 * ??  :?
 * ??  :?
 * ??  :????
 */
void EXTI_Config(void)
{

 	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//??SYSCFG??
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//PE4 ??????4
	
	/* ??EXTI_Line4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//????
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //?????
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//?????
  EXTI_Init(&EXTI_InitStructure);//??
		/*???????NVIC*/
    NVIC_Configuration();
	
}













