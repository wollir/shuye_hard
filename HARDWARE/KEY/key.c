#include "key.h"
#include "delay.h" 
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#if (ID == 1)

#else

//������ʼ������
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	// PE4 ����0
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA,GPIOEʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //KEY0 KEY1 KEY2��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
 
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)
			return 1;
	}else if(KEY0==1)
		key_up=1; 	    
 	return 0;// �ް�������
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













