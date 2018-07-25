#include "TCD1304.h"
#include "adc.h"
#include "pwm.h"
extern u8 ad_capture_flag; 
extern u32 pwm_devision;
void TCD_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟
  
  //初始化蜂鸣器对应引脚GPIOF8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//下拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
	
  GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_2);  //拉低， 
	GPIO_SetBits(GPIOD,GPIO_Pin_1);  //拉高， 
}
void wait_TcdRead(u8 *period)
{
	int i,j;
	int k;  //用来消耗时钟，略微延时
	
	ICG = 0;
	delay_ns(16);  //500ns ok! 100 - 1000ns
	SH = 1;
	delay_us(2);  // > 1us
	SH = 0;
	delay_us(5);
	ICG = 1;
	for(j = 0; j<3694; j++)
	{
		for(i = 0; i<8; i++)  //翻转 8次 也就是4个周期
			{
				Qm = ~Qm;
				k=1;//delay_ns(7);  //probobly 250ns ok  //sys clock = 2m ,so T = 500ns; 
			}
		period[j]	= (u8)(Get_Adc(ADC_Channel_5)>>2);//read one pixel;
		//	printf("实际值：%d\t\n",Get_Adc(ADC_Channel_5));
			
	}
	for(i = 0; i<8; i++)  //再加一些无关紧要的时钟
	{
		Qm = ~Qm;
	  k=1;//delay_ns(7);  //probobly 250ns
	}
}

void TcdRead(u8 *period)
{
	int i,j;
	int k;  //用来消耗时钟，略微延时
	
	//TIM_Cmd(TIM14, ENABLE);  //使能TIM14			
	ICG = 0;
	delay_ns(16);  //500ns ok! 100 - 1000ns
	//__nop();
	SH = 1;
	delay_us(2);  // > 1us
	SH = 0;
	delay_us(5);
	ICG = 1;
	// 这时候就可以通知AD开始采了
	ad_capture_flag = 1;
	while(ad_capture_flag);  //等待AD采完
	//TIM_Cmd(TIM14, DISABLE);  //使能TIM14	
//	for(j = 0; j<3694; j++)
//	{
//		for(i = 0; i<8; i++)  //翻转 8次 也就是4个周期
//			{
//				Qm = ~Qm;
//				k=1;//delay_ns(7);  //probobly 250ns ok  //sys clock = 2m ,so T = 500ns; 
//			}
//		period[j]	= (u8)(Get_Adc(ADC_Channel_5)>>2);//read one pixel;
//		//	printf("实际值：%d\t\n",Get_Adc(ADC_Channel_5));
//			
//	}
}
