#ifndef __TCD1304_H
#define __TCD1304_H	 
#include "sys.h" 
#include "delay.h"
#include "usart.h"

#define ICG PDout(0) // 
#define SH PDout(2)	// 
#define Qm PDout(1)	// 

void TCD_Init(void);//
void TcdRead(u8 *period);
#endif

















