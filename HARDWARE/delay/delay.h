#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sys.h>	  
//////////////////////////////////////////////////////////////////////////////////  
//?????????,??????,??????????
//ALIENTEK STM32F407???
//??SysTick??????????????(??ucosii)
//??delay_us,delay_ms
//????@ALIENTEK
//????:www.openedv.com
//????:2014/5/2
//??:V1.0
//????,?????
//Copyright(C) ????????????? 2014-2024
//All rights reserved
//********************************************************************************
//????
//?
////////////////////////////////////////////////////////////////////////////////// 	 
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























