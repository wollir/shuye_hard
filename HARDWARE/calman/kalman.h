#ifndef _KALMAN_H_
#define _KALMAN_H_
#include "stm32f4xx.h"
typedef struct {
	float x;  // ?????? 
	float A;  // x(n)=A*x(n-1)+u(n),u(n)~N(0,q) 
	float H;  // z(n)=H*x(n)+w(n),w(n)~N(0,r)   
	float q;  // ?????????
	float r;  // ????????? 
	float p;  // ???????
	float gain;//?????
}kalman_struct;
//extern float KalmanGain;//  ?????
//extern float EstimateCovariance;//?????
//extern float MeasureCovariance;//?????
//extern float EstimateValue;//???
//void KalmanFilterInit(void);
//double  KalmanFilter( double  Measure);
float kalman_filter(kalman_struct *kalman_lcw, float measure);
void kalman_init(kalman_struct *kalman_lcw, float init_x, float init_p);
void  shift_win_filter(unsigned char* data, int size);   //?????
#define win_size 20  //???

#endif
