#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
void USART3_Config(void);   //��ʼ�� ����USART3
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);

#endif /* __USART1_H */