#ifndef __PWM_OUTPUT_H
#define	__PWM_OUTPUT_H

#include "stm32f10x.h"

void TIM_PWM_Init(void);
void Step_M_Rps(float rps);

#endif /* __PWM_OUTPUT_H */

