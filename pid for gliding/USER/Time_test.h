#ifndef TIME_TEST_H
#define TIME_TEST_H

#include "stm32f10x.h"
 
#define START_TIME  time=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);TIM_Cmd(TIM3, ENABLE)//开启时钟，使能定时器3
#define STOP_TIME  TIM_Cmd(TIM3, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE)//关闭时钟，不是使能定时器3

void TIM3_NVIC_Configuration(void);
void TIM3_Configuration(void);


void Timer2_Init(u16 arr,u16 psc);  //定时器2初始化函数
void Timer3_Init(u16 arr,u16 psc);  //定时器3初始化函数
void Timer7_Init(u16 arr,u16 psc);  //定时器7初始化函数


#endif	/* TIME_TEST_H */
