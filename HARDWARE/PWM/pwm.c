#include "pwm.h"
#include "led.h"
#include "usart.h"
#include "adc.h"

u32 pwm_devision = 0;
u8 ad_capture_flag = 0; 
extern u8 period[3694]; //read 3694 data = 32+ 3648(real data) +14

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ��PWM ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//TIM14 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM14_PWM_Init(u32 arr,u32 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStrucure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9����Ϊ��ʱ��14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //��ʼ��PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	//TIM14->CR1  |= 0x01; // wer �������ж�ʹ��
	//TIM14->DIER  |= (1<<1);
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
	
		TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //?????3????
	//��ʼ��TIM14 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPEʹ�� 
	TIM_Cmd(TIM14, ENABLE);  //ʹ��TIM14			
	  
			NVIC_InitStrucure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn;
			NVIC_InitStrucure.NVIC_IRQChannelCmd=ENABLE;
			NVIC_InitStrucure.NVIC_IRQChannelPreemptionPriority=1;
			NVIC_InitStrucure.NVIC_IRQChannelSubPriority=1;
			NVIC_Init(&NVIC_InitStrucure);//�D�����輶����
				
	TIM_SelectOutputTrigger(TIM14,TIM_TRGOSource_Update);
}  
u32 pixel_num = 0;
void TIM8_TRG_COM_TIM14_IRQHandler(void)           //1M
{
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)==SET) //????
	{
	
			pwm_devision ++;
			if(pwm_devision == 4)  //  1m/4 = 250k
			{		
				pwm_devision = 0;
				if(ad_capture_flag == 1) //����òɵ�ʱ��
				{
					period[pixel_num++]	= (u8)(Get_Adc(ADC_Channel_5)>>2);//read one pixel;
					if(pixel_num == 3694){
						pixel_num = 0;
						ad_capture_flag = 0;
					}
					//LED0=!LED0;
					//get_ad();
				}
		}
	}
	TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //???????
}

