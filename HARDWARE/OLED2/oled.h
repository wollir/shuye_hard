//////////////////////////////////////////////////////////////////////////////////	 
//  ��������   : OLED 4�ӿ���ʾ����(STMSTM8S003F3ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              SCL   PB4
//              SDA   PB5           
//              ----------------------------------------------------------------
//******************************************************************************/��

#ifndef __OLED_H
#define __OLED_H			  	 
//#include "stm32f4xx_hal.h"
#include "type.h"

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
#define OLED_MODE 0

/**************IIC����˿�****************/

/****************SCL*********************/
#define OLED_SCLK_PORT  (GPIOB)
#define OLED_SCLK_PINS  (GPIO_PIN_4)

/****************SDA*********************/
#define OLED_SDIN_PORT  (GPIOB)
#define OLED_SDIN_PINS  (GPIO_PIN_5) 

#define OLED_SCLK_Clr() HAL_GPIO_WritePin(OLED_SCLK_PORT, OLED_SCLK_PINS ,GPIO_PIN_RESET)//GPIO_WriteLow
#define OLED_SCLK_Set() HAL_GPIO_WritePin(OLED_SCLK_PORT, OLED_SCLK_PINS ,GPIO_PIN_SET)//GPIO_WriteHigh

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(OLED_SDIN_PORT, OLED_SDIN_PINS ,GPIO_PIN_RESET)//GPIO_WriteLow
#define OLED_SDIN_Set() HAL_GPIO_WritePin(OLED_SDIN_PORT, OLED_SDIN_PINS ,GPIO_PIN_SET)//GPIO_WriteHigh

//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED�˿ڶ���----------------  				
		     
//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



