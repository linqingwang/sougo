#ifndef __Encoder_H
#define	__Encoder_H
#include "stm32f10x.h"

#define ENC_PERIOD 38912-1
#define Enc_Mid 0

void TIM2_ENC_Init(void);
uint16_t Get_Enc_Cnt(void);

#endif 

