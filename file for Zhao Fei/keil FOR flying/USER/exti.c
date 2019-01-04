#include "exti.h"
#include "delay.h"
#include "usart1.h"
#include "timer.h"
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
void EXTIX_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
 	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，使能AFIO时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTC,
		//将PC10设为浮空输入
 		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;//PC10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成浮空模式
		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC10
	//GPIOC.10 中断及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line10;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//初始化外部中断计时器

    

 
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//初始化外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级设为2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级设为1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//外部中断使能
  	NVIC_Init(&NVIC_InitStructure);  	  //初始化中断分组
 
}
long int	EXTI10_VAL=0;	//编码器读数值
long int	EXTI10_cycle=0;	//编码器读数缓存值
//外部中断程序
void EXTI15_10_IRQHandler(void)
{
	extern long int	EXTI10_VAL;	//输入捕获值
	extern long int	EXTI10_cycle;	//输入捕获值
	if(EXTI_GetITStatus(EXTI_Line10) != RESET) //确保中断已经产生
  //delay_us(100);    //消抖			 
  {
		EXTI10_VAL++;
		//printf("val00:%ld \n",EXTI10_VAL);	//打印总的高点平时间
	 EXTI_ClearITPendingBit(EXTI_Line10);  //清除LINE15线路挂起位}

	}
}
