#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

extern __IO u32	Timedelay;			//ϵͳ��ʱ�����õ�ȫ�ֱ���
void SysTick_Init(void);
void Delay_ms(__IO u32 nTime);

#endif /* __SYSTICK_H */

