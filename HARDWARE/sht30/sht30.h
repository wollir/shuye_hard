#ifndef __SHT30_H
#define __SHT30_H
#include"myiic.h"

typedef struct 
{
	u8 tempL;
	u8 tempH;
	u8 humiL;
	u8 humiH;
}sht_data;


void start_mesure(void);
sht_data sht30_read(void);
//u8 aa();
#endif
















