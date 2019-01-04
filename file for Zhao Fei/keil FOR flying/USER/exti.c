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
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;       //����PB8Ϊ���ж�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 // PB8����Ϊ��������ģʽ
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
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�Ͻ����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�Ͻ����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);  	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�Ͻ����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}
void EXTIX_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
 	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ�ʹ��AFIOʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTC,
		//��PC10��Ϊ��������
 		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;//PC10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���óɸ���ģʽ
		GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC10
	//GPIOC.10 �жϼ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line10;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//��ʼ���ⲿ�жϼ�ʱ��

    

 
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//��ʼ���ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//��ռ���ȼ���Ϊ2
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ���Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//�ⲿ�ж�ʹ��
  	NVIC_Init(&NVIC_InitStructure);  	  //��ʼ���жϷ���
 
}
long int	EXTI10_VAL=0;	//����������ֵ
long int	EXTI10_cycle=0;	//��������������ֵ
//�ⲿ�жϳ���
void EXTI15_10_IRQHandler(void)
{
	extern long int	EXTI10_VAL;	//���벶��ֵ
	extern long int	EXTI10_cycle;	//���벶��ֵ
	if(EXTI_GetITStatus(EXTI_Line10) != RESET) //ȷ���ж��Ѿ�����
  //delay_us(100);    //����			 
  {
		EXTI10_VAL++;
		//printf("val00:%ld \n",EXTI10_VAL);	//��ӡ�ܵĸߵ�ƽʱ��
	 EXTI_ClearITPendingBit(EXTI_Line10);  //���LINE15��·����λ}

	}
}
