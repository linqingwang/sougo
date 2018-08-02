#include "exti.h"

void EXTI_PB89_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PB8) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);

	/* EXTI line gpio config(PB8) */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;       //配置PB8为线中断
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 // PB8引脚为浮空输入模式
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 // 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 // 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	/* EXTI line(PB8) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);  	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}
