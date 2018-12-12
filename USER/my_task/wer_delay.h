/**
  ******************************************************************************
  * @file    Delay.h
  * @author  ���
  * @version V1.2
  * @date    18-June-2018
  * @brief   ���ͷ�ļ�������STM32F407ZGT6���������ʱ��صĺ���. 
  ******************************************************************************
  * @attention
  *		�ٷ����ļ����вο�����
  *		Ϊ��֤��ʱϵͳ�ľ�׼��Ҫ��ϵͳ��Ƶ��Խ��Խ��
  *		Ϊ��֤��ʱϵͳ������֧�֣�ϵͳ��Ƶ������8MHz
  *		ʹ��CM4ϵͳ��ʱ�����ж����ȼ����
  *
  * @����˵��
  *		֧��us��ms����ʱ���Ҿ��ȴ�����
  *		֧��OS��������Ӱ��OS��ʹ�ã��¸��汾���ӣ�
  *
  ******************************************************************************
  */
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __werDELAY_H
#define __werDELAY_H
 
 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_rcc.h"
#include "core_cm4.h"
#include "stm32f4xx_it.h"
#include "misc.h"
 
 
 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
 
void Delay_Init(void);					//��ʱϵͳ��ʼ��
void Delay_Us(u16 us);				//΢����ʱ
void Delay_Ms(u16 ms);				//������ʱ
void Delay_Us_2(u32 us);				//΢����ʱ����Χ����
 
 
 
 
#endif /* __DELAY_H */
/************************ (C) COPYRIGHT ��� *****END OF FILE****/