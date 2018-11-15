#ifndef MAINH
#define MAINH

#include "stm32f4xx.h"

/* Data definitions */
#define CCDSize 3694
#define RxDataSize 12
#define RealDataSize 3648
#define UsedSize 60
#define ID 3
#define CHANNAL 0X17
#define wireless
/* CCD master clock in Hz */
/* The values presented here are the prescalable frequencies between
   1-2MHz with the STM32F401RE running at 84 MHz. Other frequencies
   are achievable when prescaling APB1 differently. 1 MHz does not
   seem to work well. */
//#define CCD_fm 1000000
//#define CCD_fm 1400000
//#define CCD_fm 1500000
//define CCD_fm 1680000
//#define CCD_fm 1750000
#define CCD_fm 2000000

/*  Comply with the CCD's timing requirements:
	The delay is dependent on the CCD_fM. These values appear to work:
		For 1.4-2.0 MHz use: ICG_delay = 11 and SH_delay = 12
		For 1.0 MHz use: ICG_delay = 12 and SH_delay = 12   */
#define SH_delay 12
#define ICG_delay 11
#define fm_delay 3



/* UART definitions */
#define BAUDRATE 9600

#endif
