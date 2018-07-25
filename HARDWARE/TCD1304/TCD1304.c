#include "TCD1304.h"
#include "adc.h"
#include "pwm.h"
extern u8 ad_capture_flag; 
extern u32 pwm_devision;
void TCD_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��
  
  //��ʼ����������Ӧ����GPIOF8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO
	
  GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_2);  //���ͣ� 
	GPIO_SetBits(GPIOD,GPIO_Pin_1);  //���ߣ� 
}
void wait_TcdRead(u8 *period)
{
	int i,j;
	int k;  //��������ʱ�ӣ���΢��ʱ
	
	ICG = 0;
	delay_ns(16);  //500ns ok! 100 - 1000ns
	SH = 1;
	delay_us(2);  // > 1us
	SH = 0;
	delay_us(5);
	ICG = 1;
	for(j = 0; j<3694; j++)
	{
		for(i = 0; i<8; i++)  //��ת 8�� Ҳ����4������
			{
				Qm = ~Qm;
				k=1;//delay_ns(7);  //probobly 250ns ok  //sys clock = 2m ,so T = 500ns; 
			}
		period[j]	= (u8)(Get_Adc(ADC_Channel_5)>>2);//read one pixel;
		//	printf("ʵ��ֵ��%d\t\n",Get_Adc(ADC_Channel_5));
			
	}
	for(i = 0; i<8; i++)  //�ټ�һЩ�޹ؽ�Ҫ��ʱ��
	{
		Qm = ~Qm;
	  k=1;//delay_ns(7);  //probobly 250ns
	}
}

void TcdRead(u8 *period)
{
	int i,j;
	int k;  //��������ʱ�ӣ���΢��ʱ
	
	//TIM_Cmd(TIM14, ENABLE);  //ʹ��TIM14			
	ICG = 0;
	delay_ns(16);  //500ns ok! 100 - 1000ns
	//__nop();
	SH = 1;
	delay_us(2);  // > 1us
	SH = 0;
	delay_us(5);
	ICG = 1;
	// ��ʱ��Ϳ���֪ͨAD��ʼ����
	ad_capture_flag = 1;
	while(ad_capture_flag);  //�ȴ�AD����
	//TIM_Cmd(TIM14, DISABLE);  //ʹ��TIM14	
//	for(j = 0; j<3694; j++)
//	{
//		for(i = 0; i<8; i++)  //��ת 8�� Ҳ����4������
//			{
//				Qm = ~Qm;
//				k=1;//delay_ns(7);  //probobly 250ns ok  //sys clock = 2m ,so T = 500ns; 
//			}
//		period[j]	= (u8)(Get_Adc(ADC_Channel_5)>>2);//read one pixel;
//		//	printf("ʵ��ֵ��%d\t\n",Get_Adc(ADC_Channel_5));
//			
//	}
}
