#ifndef __POWER_H
#define __POWER_H
#include "stm32f1xx_hal.h"

void POWER_Init(void);

void POWER_Get_Data(float *U, float *I,float *P);


#endif	/* __POWER_H */



