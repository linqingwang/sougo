#ifndef TIME_TEST_H
#define TIME_TEST_H

#include "stm32f10x.h"
 
#define START_TIME  time=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);TIM_Cmd(TIM3, ENABLE)//����ʱ�ӣ�ʹ�ܶ�ʱ��3
#define STOP_TIME  TIM_Cmd(TIM3, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE)//�ر�ʱ�ӣ�����ʹ�ܶ�ʱ��3

void TIM3_NVIC_Configuration(void);
void TIM3_Configuration(void);


void Timer2_Init(u16 arr,u16 psc);  //��ʱ��2��ʼ������
void Timer3_Init(u16 arr,u16 psc);  //��ʱ��3��ʼ������
void Timer7_Init(u16 arr,u16 psc);  //��ʱ��7��ʼ������


#endif	/* TIME_TEST_H */
