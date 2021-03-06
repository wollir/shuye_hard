#include "main.h"
#include "wer_task.h"
#include "UART_conf.h"
#include "key.h"
#include "delay.h"
__IO u8 echo_pc = 0;
__IO u8 isalert = 0; //通知上位机是否已经报警
__IO u8 isalerted = 0;
/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

extern __IO uint32_t SH_period;
extern __IO uint32_t ICG_period;
extern __IO uint16_t aTxBuffer[CCDSize];
extern __IO uint8_t aRxBuffer[RxDataSize];
extern __IO uint8_t nRxBuffer[RxDataSize];

extern __IO uint8_t change_exposure_flag;
extern __IO uint8_t transmit_data_flag;
//extern __IO uint8_t pc_ready_flag;
extern __IO uint8_t pulse_counter;
extern __IO uint8_t CCD_flushed;

static u8 isRightStart = 0;// 启动标志，处理刚开机就发东西的问题

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}
void EXTI4_IRQHandler(void)
{   
	  /*延时消抖*/
		delay_ms(10);	  		 
     /*检查指定的EXTI13线路触发请求发生与否*/	
    if(EXTI_GetITStatus(EXTI_Line4) != RESET)
		{   
			if(isalert==1){
				isalert = 0; //不报警了
				isalerted = 1; // 报警完成
				beep(1);
			}
		}
		/*清除EXTI13线路挂起位*/
	 EXTI_ClearITPendingBit(EXTI_Line4); 

}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s).                         */
/******************************************************************************/

/*	This interrupt is envoked when UART-reception is completed */
void DMA1_Stream5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5)) 
	{
		int i = 0,j=0;
		/* Clear DMA Stream Transfer Complete interrupt */
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);  
		
		/* Wait a short while - to let DMA finish ..or something */
		
		//for(j=0;j<100;j++)
		for (i=0;i<8400;i++);
		leda(0);
		/* Sort aRxBuffer into nRxBuffer */	
		sort_aRxBuffer();

		if(nRxBuffer[4]==1 && ((nRxBuffer[2]<<8)|nRxBuffer[3]) == ID) //说明是要求响应的  [3]是1说明是召唤命令
		{
				echo_pc = 1;
		}		
		/* Check the key before doing anything */
	 	else if ((nRxBuffer[0]==69)&&(nRxBuffer[1]==82)&&(((nRxBuffer[10]<<8)|nRxBuffer[11])==ID))
		{ 
				/* reset the key */
				nRxBuffer[0] = 0;
				nRxBuffer[1] = 0;
				isalert = nRxBuffer[2];  //通知下位机是否报警
			if(isalert){  //蜂鸣器响起
				beep(0);
			}
				/* set flags for main-loop */
				change_exposure_flag = 1;
				//pc_ready_flag = nRxBuffer[10];
				back_led(0); //收到正确得东西，将背光源打开
		}
	}
}


/* This interrupt is envoked when the ADC has finished reading the CCD */
void DMA2_Stream0_IRQHandler(void)
{
	/* Test for DMA Stream Transfer Complete interrupt */
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
	{
		/* Clear DMA Stream Transfer Complete interrupt pending bit */
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);  
    
		/* Stop TIM4 and thus the ADC */
		TIM4->CR1 &= (uint16_t)~TIM_CR1_CEN;

		/* Set the transmit_data_flag */
		if(!isRightStart){
			isRightStart = 1;
			return;
		}
			transmit_data_flag = 1;
	}
}

/* 	This interrupt is envoked when the ICG-pulse is sent,
	ie. when the CCD starts to output */
void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update))
	{	
		/* Clear TIM5 update interrupt */
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		if (pulse_counter == 6)
		{
			/* Restart TIM4 as this gets the ADC running again */
			TIM4->CR1 |= TIM_CR1_CEN;
		}
		else if (pulse_counter == 3)
		{
			CCD_flushed = 1;
		}
		pulse_counter++;
		
		/* prevent overflow */
		if (pulse_counter > 10)
			pulse_counter = 10;

		/* Flash the led to the beat of ICG */
		//GPIOF->ODR ^= GPIO_Pin_9;
	}

}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
