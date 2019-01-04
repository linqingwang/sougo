#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

extern __IO u32	Timedelay;			//系统定时器所用的全局变量
void SysTick_Init(void);
void Delay_ms(__IO u32 nTime);

#endif /* __SYSTICK_H */

