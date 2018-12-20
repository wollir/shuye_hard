#include "kalman.h"
#include "math.h"

#define win_size 20  //???
//unsigned char win_temp[win_size];
void  shift_win_filter(unsigned char* data, int size)   //?????
{
    double sum = 0;
    int state,i;
    for ( state = 0; state < size - win_size+1; state++)
    {
        sum = 0;
        for (i = state; i < win_size + state; i++)
        {
            //win_temp[i] = data[i];
            //sum += win_temp[i];
            sum += data[i];
        }
        sum /= win_size;   //???
        data[state] = sum; //??????????????
    }
    for (i = state; i <size; i++)
    {
        data[i] = sum;
    } //????????????????
}
void kalman_init(kalman_struct *kalman_lcw, float init_x, float init_p)
{
	kalman_lcw->x = init_x;//???????,??????????
	kalman_lcw->p = init_p;//????????????????
	kalman_lcw->A = 1;
	kalman_lcw->H = 1;
	kalman_lcw->q = 0.1;//¹ý³ÌÔëÉù¡£
	kalman_lcw->r = 0.1;//²âÁ¿ÔëÉù¡£
	//???????????
}

/**
*kalman_filter - ??????
*@kalman_lcw:??????
*@measure;???
*???????
*/
float kalman_filter(kalman_struct *kalman_lcw, float measure)
{
	/* Predict */
	kalman_lcw->x = kalman_lcw->A * kalman_lcw->x;
	kalman_lcw->p = kalman_lcw->A * kalman_lcw->A * kalman_lcw->p + kalman_lcw->q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */

	/* Measurement */
	kalman_lcw->gain = kalman_lcw->p * kalman_lcw->H / (kalman_lcw->p * kalman_lcw->H * kalman_lcw->H + kalman_lcw->r);
	kalman_lcw->x = kalman_lcw->x + kalman_lcw->gain * (measure - kalman_lcw->H * kalman_lcw->x);
	kalman_lcw->p = (1 - kalman_lcw->gain * kalman_lcw->H) * kalman_lcw->p;

	return kalman_lcw->x;
}
