/**
  ******************************************************************************
  * @file    Delay.c
  * @author  李航兵
  * @version V1.2
  * @date    18-June-2018
  * @brief   这个文件包含以下函数用于操作STM32F407ZGT6开发板的延时功能
  *           + 延时系统初始化
  *           + 微秒延时
  *           + 毫秒延时
  *
  *  @verbatim
  *
  *
    ===========================================================================
                         ##### How to use this driver #####
    ===========================================================================
      [..]
      (#) 初始化延时系统，调用Delay_Init()
      (#) 延时
         (++) 毫秒: Delay_Ms()
         (++) 微秒: Delay_Init()
    @endverbatim
  ******************************************************************************
  * @attention
  *		官方库文件里有参考代码
  *		为保证延时系统的精准，要求系统主频率越高越好
  *		为保证延时系统的完整支持，系统主频率至少8MHz
  *		使用CM4系统定时器，中断优先级最低
  *		使用此文件后，禁止再修改SysTick计时器及其设置
  *
  *
  *
  *
  *
  *
  * <h2><center>&copy; COPYRIGHT 2018 李航兵</center></h2>
  * @更新说明
  *		支持us、ms级延时，且精度大大提高
  *		支持OS，基本不影响OS的使用（下个版本添加）
  *
  *
  *
  *
  *
  *
  ******************************************************************************
  */
  
  /* Includes ------------------------------------------------------------------*/
#include "wer_delay.h"
 
 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 
 
u8 per_us;				//每1us定时器节拍
u32 per_ms;				//每1ms节拍，注意168MHz下值为168000，需要32位，移植自STM32F0，此处谨慎
 
 
 
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
 
 
/**
  * @brief  延时系统初始化.
  * @param  None.
  * @retval None
  */
void Delay_Init()
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
		//这里有个问题，本想自动化获取时钟频率的，不料在该函数中有如下声明
	/*
	HSE_VALUE is a constant defined in stm32f4xx.h file (default value
  *                25 MHz), user has to ensure that HSE_VALUE is same as the real
  *                frequency of the crystal used. Otherwise, this function may
  *                have wrong result.
	*/
	//也就是后来测试时发现延时总是为输入的3倍左右
	//后修改了库文件，可参看对应目录的readme文件获取修改记录
	if(SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000))		//1ms一次中断
		while(1);	
	
	per_ms=SysTick->LOAD;							//每1ms节拍,亦即重载值
	per_us=per_ms/1000;		//每1us定时器节拍
 
}
 
 
 
/**
  * @brief  微秒延时.
  * @param  延时的微秒数，约定范围1~390，“禁止其他值”.
  * @note   存在一定误差，主要是函数调用+部分计算.
  * @retval None
  */
void Delay_Us(u16 us)				//微秒延时
{
	u32 ticks_old=SysTick->VAL;		//前一个计数值
	u32 ticks_new;					//后一个计数值
	u16 ticks_sum=0;				//已经经过的节拍
	u16 ticks_delta=us*per_us;		//需要经过的节拍
	if(us>390) return;				//计时不允许超过390us，超过390us请使用Delay_Us_2
	while(1)
	{
		ticks_new=SysTick->VAL;	
		if(ticks_new!=ticks_old)
		{
			if(ticks_new<ticks_old)ticks_sum+=ticks_old-ticks_new;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else ticks_sum+=per_ms-ticks_new+ticks_old;	    
			ticks_old=ticks_new;
			if(ticks_sum>=ticks_delta)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	}
}

 
/**
  * @brief  毫秒延时.
  * @param  延时的毫秒数，约定范围1~25000，“禁止其他值”.
  * @note   存在一定误差，主要是函数调用+部分计算.
  * @retval None
  */
void Delay_Ms(u16 ms)				//毫秒延时
{
	u32 ticks_old=SysTick->VAL;		//前一个计数值
	u32 ticks_new;					//后一个计数值
	u32 ticks_sum=0;				//已经经过的节拍
	u32 ticks_delta=ms*per_ms;			//需要经过的节拍
	if(ms>25000) return;				//计时不允许超过25000ms，超过25000ms请多次使用
	while(1)
	{
		ticks_new=SysTick->VAL;	
		if(ticks_new!=ticks_old)
		{
			if(ticks_new<ticks_old)ticks_sum+=ticks_old-ticks_new;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else ticks_sum+=per_ms-ticks_new+ticks_old;	    
			ticks_old=ticks_new;
			if(ticks_sum>=ticks_delta)break;			//时间超过/等于要延迟的时间,则退出.
		}
	}
}
 
 
 
 
/**
  * @brief  微秒延时，范围更大.
  * @param  延时的微秒数，约定范围1~25000000，“禁止其他值”.
  * @note   存在一定误差，主要是函数调用+部分计算.
  * @retval None
  */
void Delay_Us_2(u32 us)				//微秒延时，范围更大
{
	u32 ticks_old=SysTick->VAL;		//前一个计数值
	u32 ticks_new;					//后一个计数值
	u32 ticks_sum=0;				//已经经过的节拍
	u32 ticks_delta=us*per_us;		//需要经过的节拍
	if(us>25000000) return;				//计时不允许超过25000000us
	while(1)
	{
		ticks_new=SysTick->VAL;	
		if(ticks_new!=ticks_old)
		{
			if(ticks_new<ticks_old)ticks_sum+=ticks_old-ticks_new;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else ticks_sum+=per_ms-ticks_new+ticks_old;	    
			ticks_old=ticks_new;
			if(ticks_sum>=ticks_delta)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	}
}
inline void diy_delay_1u(int us)
{
	int i = 0;
	for(i = 0; i < us;i++)
		;
} 
 
 
/**
  * @brief  系统定时器中断.
  * @param  None.
  * @note   一般给OS用.
  * @retval None
  */
//void SysTick_Handler(void)		//系统定时器中断
//{
//	
//}
// 

