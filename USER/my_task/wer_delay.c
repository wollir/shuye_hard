/**
  ******************************************************************************
  * @file    Delay.c
  * @author  ���
  * @version V1.2
  * @date    18-June-2018
  * @brief   ����ļ��������º������ڲ���STM32F407ZGT6���������ʱ����
  *           + ��ʱϵͳ��ʼ��
  *           + ΢����ʱ
  *           + ������ʱ
  *
  *  @verbatim
  *
  *
    ===========================================================================
                         ##### How to use this driver #####
    ===========================================================================
      [..]
      (#) ��ʼ����ʱϵͳ������Delay_Init()
      (#) ��ʱ
         (++) ����: Delay_Ms()
         (++) ΢��: Delay_Init()
    @endverbatim
  ******************************************************************************
  * @attention
  *		�ٷ����ļ����вο�����
  *		Ϊ��֤��ʱϵͳ�ľ�׼��Ҫ��ϵͳ��Ƶ��Խ��Խ��
  *		Ϊ��֤��ʱϵͳ������֧�֣�ϵͳ��Ƶ������8MHz
  *		ʹ��CM4ϵͳ��ʱ�����ж����ȼ����
  *		ʹ�ô��ļ��󣬽�ֹ���޸�SysTick��ʱ����������
  *
  *
  *
  *
  *
  *
  * <h2><center>&copy; COPYRIGHT 2018 ���</center></h2>
  * @����˵��
  *		֧��us��ms����ʱ���Ҿ��ȴ�����
  *		֧��OS��������Ӱ��OS��ʹ�ã��¸��汾��ӣ�
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
 
 
u8 per_us;				//ÿ1us��ʱ������
u32 per_ms;				//ÿ1ms���ģ�ע��168MHz��ֵΪ168000����Ҫ32λ����ֲ��STM32F0���˴�����
 
 
 
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
 
 
/**
  * @brief  ��ʱϵͳ��ʼ��.
  * @param  None.
  * @retval None
  */
void Delay_Init()
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
		//�����и����⣬�����Զ�����ȡʱ��Ƶ�ʵģ������ڸú���������������
	/*
	HSE_VALUE is a constant defined in stm32f4xx.h file (default value
  *                25 MHz), user has to ensure that HSE_VALUE is same as the real
  *                frequency of the crystal used. Otherwise, this function may
  *                have wrong result.
	*/
	//Ҳ���Ǻ�������ʱ������ʱ����Ϊ�����3������
	//���޸��˿��ļ����ɲο���ӦĿ¼��readme�ļ���ȡ�޸ļ�¼
	if(SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000))		//1msһ���ж�
		while(1);	
	
	per_ms=SysTick->LOAD;							//ÿ1ms����,�༴����ֵ
	per_us=per_ms/1000;		//ÿ1us��ʱ������
 
}
 
 
 
/**
  * @brief  ΢����ʱ.
  * @param  ��ʱ��΢������Լ����Χ1~390������ֹ����ֵ��.
  * @note   ����һ������Ҫ�Ǻ�������+���ּ���.
  * @retval None
  */
void Delay_Us(u16 us)				//΢����ʱ
{
	u32 ticks_old=SysTick->VAL;		//ǰһ������ֵ
	u32 ticks_new;					//��һ������ֵ
	u16 ticks_sum=0;				//�Ѿ������Ľ���
	u16 ticks_delta=us*per_us;		//��Ҫ�����Ľ���
	if(us>390) return;				//��ʱ��������390us������390us��ʹ��Delay_Us_2
	while(1)
	{
		ticks_new=SysTick->VAL;	
		if(ticks_new!=ticks_old)
		{
			if(ticks_new<ticks_old)ticks_sum+=ticks_old-ticks_new;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else ticks_sum+=per_ms-ticks_new+ticks_old;	    
			ticks_old=ticks_new;
			if(ticks_sum>=ticks_delta)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}
}

 
/**
  * @brief  ������ʱ.
  * @param  ��ʱ�ĺ�������Լ����Χ1~25000������ֹ����ֵ��.
  * @note   ����һ������Ҫ�Ǻ�������+���ּ���.
  * @retval None
  */
void Delay_Ms(u16 ms)				//������ʱ
{
	u32 ticks_old=SysTick->VAL;		//ǰһ������ֵ
	u32 ticks_new;					//��һ������ֵ
	u32 ticks_sum=0;				//�Ѿ������Ľ���
	u32 ticks_delta=ms*per_ms;			//��Ҫ�����Ľ���
	if(ms>25000) return;				//��ʱ��������25000ms������25000ms����ʹ��
	while(1)
	{
		ticks_new=SysTick->VAL;	
		if(ticks_new!=ticks_old)
		{
			if(ticks_new<ticks_old)ticks_sum+=ticks_old-ticks_new;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else ticks_sum+=per_ms-ticks_new+ticks_old;	    
			ticks_old=ticks_new;
			if(ticks_sum>=ticks_delta)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}
	}
}
 
 
 
 
/**
  * @brief  ΢����ʱ����Χ����.
  * @param  ��ʱ��΢������Լ����Χ1~25000000������ֹ����ֵ��.
  * @note   ����һ������Ҫ�Ǻ�������+���ּ���.
  * @retval None
  */
void Delay_Us_2(u32 us)				//΢����ʱ����Χ����
{
	u32 ticks_old=SysTick->VAL;		//ǰһ������ֵ
	u32 ticks_new;					//��һ������ֵ
	u32 ticks_sum=0;				//�Ѿ������Ľ���
	u32 ticks_delta=us*per_us;		//��Ҫ�����Ľ���
	if(us>25000000) return;				//��ʱ��������25000000us
	while(1)
	{
		ticks_new=SysTick->VAL;	
		if(ticks_new!=ticks_old)
		{
			if(ticks_new<ticks_old)ticks_sum+=ticks_old-ticks_new;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else ticks_sum+=per_ms-ticks_new+ticks_old;	    
			ticks_old=ticks_new;
			if(ticks_sum>=ticks_delta)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
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
  * @brief  ϵͳ��ʱ���ж�.
  * @param  None.
  * @note   һ���OS��.
  * @retval None
  */
//void SysTick_Handler(void)		//ϵͳ��ʱ���ж�
//{
//	
//}
// 

