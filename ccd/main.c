#include "main.h"
#include "key.h"
#include "wer_task.h"
#include "timer_conf.h"
#include "delay.h"
#include "ADC_conf.h"
#include "UART_conf.h"
#include "kalman.h"
#include "myiic.h"
#include "sht30.h"

void virtual_GND(void);
void flush_CCD(void);
void NVIC_conf(void);
extern __IO u8 isalert; //通知上位机是否已经报警
extern __IO u8 echo_pc;
extern __IO u8 isalerted;//下位机是否已经报警完成。同志上位机输液结束
__IO uint32_t SH_period = 25;
__IO uint32_t ICG_period = 500000;
//__IO uint32_t SH_period = 250;
//__IO uint32_t ICG_period = 5000000;

__IO uint16_t aTxBuffer[CCDSize];
__IO uint8_t aRxBuffer[RxDataSize] = {0};
__IO uint8_t nRxBuffer[RxDataSize] = {0};

__IO uint8_t change_exposure_flag = 0;
__IO uint8_t transmit_data_flag = 0;
__IO uint8_t pulse_counter = 0;
__IO uint8_t CCD_flushed = 0;

u8 real_data[RealDataSize];
u8 sorted_data[UsedSize];
u8 echo_data[UsedSize];
float after_calman[RealDataSize] = {0};
/* TIM Configuration
	TIM2/5 are 32 bit and will serve the ICG and SH pulses that may require very long periods.
	TIM3/4 are 16 bit and will serve the fM (master clock) and ADC clock. 

	TIM2 drives SH on PB10
	TIM3 drives fM on PB1 
	TIM4 paces the ADC
	TIM5 drives ICG on PA1

	ADC-In is on PC3 */

/* UART Configuration
	Tx on PA3
	Rx on PA2 */

/* Other GPIOs
	PA0, PB0, PB2 and PC2 are driven low
	PA5 (LED) is enabled (but driven low by default) */
//u8 kalman_test[10] = {20,40,70,100,200,170,110,150,190,130};
//u8 kalman_out[10] ={0}; 
int main(void)
{
	int  i= 0;
	sht_data temp_hum;
	kalman_struct kal_stu;
	kalman_struct *kal = &kal_stu;
	/* virtual_GND() enables GPIOA, GPIOB and GPIOC clocks */
	virtual_GND();
	NVIC_conf();
	//led_B2_init();
	//GPIO_SetBits(GPIOB,GPIO_Pin_2);//GPIOF9,F10???,??
	//GPIO_ResetBits(GPIOB,GPIO_Pin_2);
	/* Setup CCD_fM (TIM3) and ADC-timer (TIM4) */
	get_Timer_clocks();
	TIM_CCD_fM_conf();
	TIM_ADC_conf();

/* Setup UART */
	USART2_conf();
	USART_GetFlagStatus(USART2, USART_FLAG_TC); //解决第一个字节发不出去的问题
/* Setup ADC + ADC-DMA */
	ADC1_conf();
/* Setup ICG (TIM5) and SH (TIM2) */
	TIM_ICG_SH_conf();
	delay_init(84);
	KEY_Init();
	EXTI_Config();//按键中断初始化
	//flush_CCD();
	kalman_init(kal,150,0.01);
	IIC_Init();
	
	while(1) 	
	{		
		if (change_exposure_flag == 1)
		{
			/* reset flag */
			change_exposure_flag = 0;

			flush_CCD();
			/* set new integration time */
//			ICG_period = aRxBuffer[6]<<24|aRxBuffer[7]<<16|aRxBuffer[8]<<8|aRxBuffer[9];
//			SH_period = aRxBuffer[2]<<24|aRxBuffer[3]<<16|aRxBuffer[4]<<8|aRxBuffer[5];
			ICG_period = nRxBuffer[6]<<24|nRxBuffer[7]<<16|nRxBuffer[8]<<8|nRxBuffer[9];   // 设置TIM5的 周期
			SH_period = nRxBuffer[2]<<24|nRxBuffer[3]<<16|nRxBuffer[4]<<8|nRxBuffer[5];    // 设置TIM2 的周期


			/*	Disable ICG (TIM5) and SH (TIM2) before reconfiguring*/
			TIM_Cmd(TIM2, DISABLE);
			TIM_Cmd(TIM5, DISABLE);

			/* 	Reconfigure TIM2 and TIM5 */
			TIM_ICG_SH_conf();
		}
//		if ((transmit_data_flag == 1)&&(pc_ready_flag == 1))
		if(transmit_data_flag == 1)
		{
			/* reset flags */
			transmit_data_flag = 0;
//			pc_ready_flag = 0;

			/* Transmit data */
			// wer_send('o');
		  // wer_send('k');
			// i can do anything else here.
			for(i = 32;i<3680; i++){
				real_data[i-32] = (u8)(aTxBuffer[i]>>4);   // 将采集到的 16 位数据（实际上是12位的AD值强制转换成8位，其他4位不要了）			
				//real_data[i-32] = (u8)(aTxBuffer[i]|0xff);
			}		
			for(i = 0;i<RealDataSize;i++){
				real_data[i] = 255-real_data[i];
			}			
			SortFrom3648(real_data,sorted_data,UsedSize);
			for(i = 0;i<UsedSize;i++) //卡尔曼滤波
			{
				sorted_data[i] = (u8)kalman_filter(kal,(float)sorted_data[i]);
			}
			start_mesure();
			temp_hum = sht30_read();
#ifdef wireless			
		  wer_send(0x01);wer_send(0x00);wer_send(CHANNAL);// 主机地址
#endif
			wer_send(0xfe);wer_send(0x01);
			send_data(sorted_data,UsedSize);
			send_TempHumi(temp_hum);
			wer_send(ID>>8);
			wer_send(ID&0xff);
			wer_send(isalerted);isalerted = 0; //报警标志再设置回来
			wer_send(0); //预留
			back_led(1); //将背光源 关闭
			//UART2_Tx_DMA();
			leda(1);
		}
		if(echo_pc){   //扫描时候回应
#ifdef wireless
			wer_send(0x01);wer_send(0x00);wer_send(CHANNAL);// 主机地址
#endif
			wer_send(0xff);wer_send(0x02);// 扫描帧头
		  echo_data[0] =ID>>8;
			echo_data[1] =ID&0xff;
			send_data(echo_data,UsedSize);//凑字数
			wer_send(8);//凑字数
			wer_send(8);wer_send(8);wer_send(8);wer_send(8);wer_send(8);wer_send(8);wer_send(8);
			echo_pc = 0;
			//delay_ms(200);
			leda(1);
		}
	}
}
/* 	To keep noise-level on ADC-in down, the following GPIO's are
	set as output, driven low and physically connected to GND:
		PA0 and PC2 which are physically close to PC3 (ADC-in)
		PB0 and PB2 which are physically close to PB1 - the most busy GPIO (fM) */
void virtual_GND(void)
{
	GPIO_InitTypeDef    	GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	/* 	Clock the GPIOs */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  

	/* PA0 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* PC2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* PB0 and PB2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Setup LED (PA6) Setup LED (PA7) Setup backgruand—_LED (PA4)*/  //---PF9
#if	(ID == 1)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_SetBits(GPIOA,GPIO_Pin_6|GPIO_Pin_7);
#else
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
	GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
#endif
}
/* Run this function prior to datacollection */
void flush_CCD()
{
	/* Set exposure very low */
	ICG_period = 15000;
	SH_period = 25;

	/*	Disable ICG (TIM5) and SH (TIM2) before reconfiguring*/
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(TIM5, DISABLE);

	/*	Reset flags and counters */
	CCD_flushed = 0;
	pulse_counter = 0;

	/* 	Reconfigure TIM2 and TIM5 */
	TIM_ICG_SH_conf();

	/*	Block until CCD is properly flushed */
	while(CCD_flushed == 0);
}


/* Configure interrupts */
void NVIC_conf(void)
{
	NVIC_InitTypeDef		NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ICG (TIM5) IRQ */
	/* The TIM5 update interrupts starts TIM4 and ADC */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* ADC-DMA IRQ */
	/* DMA1 Transfer complete interrupt stops TIM4 and ADC */
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* USART2-DMA-Rx IRQ */
	/* DMA1 Transfer complete interrupt checks incoming data */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


