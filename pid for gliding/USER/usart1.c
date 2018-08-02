/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��usart1.c
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ��������
 *           ��ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�         
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * ��汾  ��ST3.5.0
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/	
#include "usart1.h"
#include "misc.h"

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 


//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;    

	USART_SendData(USART3, (unsigned char) ch);
  while (!(USART3->SR & USART_FLAG_TXE));
  
	return ch;
}
#endif 



/*
 * ��������USART1_Config
 * ����  ��USART1 GPIO ����,����ģʽ����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
}


//����3��ʼ��
void USART3_Config(void)   //��ʼ�� ����USART3
{
	GPIO_InitTypeDef GPIO_InitStructure;	  //���ڶ˿����ýṹ�����
	USART_InitTypeDef USART_InitStructure;	  //���ڲ������ýṹ�����

	//ʹ�� USART3 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//�򿪴��ڸ���ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //��PB�˿�ʱ��
  	
	//��USART3 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				  //ѡ���ĸ�IO�� ��ѡ��PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�趨IO�ڵ�����ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);    				  //��ʼ��GPIOB
	
	//��USART3 Rx�����սţ���GPIO����Ϊ��������ģʽ														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //ѡ���ĸ�IO�� ��ѡ��PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //��ʼ��GPIOA
	  
	//����USART3����
	USART_InitStructure.USART_BaudRate = 115200;	                    //����������115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART3, &USART_InitStructure);                       //��ʼ��USART3
	
  //�򿪷����жϺͽ����ж�(�����Ҫ�ж�)
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART3�����ж�

	//ʹ�� USART3�� �������
	USART_Cmd(USART3, ENABLE);                             // USART3ʹ��

	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
  USART_ClearFlag(USART3, USART_FLAG_TC);                //�崮��3���ͱ�־
}


void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//	//����3�ж����ȼ�����(RS485)
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			   //ָ���ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	   //��ռ���ȼ�0������ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;             //ָ����Ӧ���ȼ���1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	               //���ж�
	NVIC_Init(&NVIC_InitStructure);							       //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  //�ж�ͨ������ʱ���ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//�����ȼ�Ϊ3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ��TIM2�ж�
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//void USART2_Config(void)   //��ʼ�� ����USART2
//{
//	GPIO_InitTypeDef    GPIO_InitStructure;	   //���ڶ˿����ýṹ�����
//	USART_InitTypeDef   USART_InitStructure;   //���ڲ������ýṹ�����

//	//ʹ�� USART2 ʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//�򿪴��ڸ���ʱ��
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);   //��PA�˿�ʱ��

//	//��USART2 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ   
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				  //ѡ���ĸ�IO�� ��ѡ��PA2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�趨IO�ڵ�����ٶ�Ϊ50MHz
//	GPIO_Init(GPIOA, &GPIO_InitStructure);    				  //��ʼ��GPIOA

//	//��USART2 Rx�����սţ���GPIO����Ϊ��������ģʽ														  
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				  //ѡ���ĸ�IO�� ��ѡ��PA3
//	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //��������
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);                    //��ʼ��GPIOA
//	  
//	//����USART2����
//	USART_InitStructure.USART_BaudRate = 115200;	                    //���������ã�115200
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //����λ�����ã�8λ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //ֹͣλ���ã�1λ
//	USART_InitStructure.USART_Parity = USART_Parity_No ;            //�Ƿ���żУ�飺��
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
//	USART_Init(USART2, &USART_InitStructure);                       //��ʼ��USART2
//	
//	//�򿪷����жϺͽ����ж�(�����Ҫ�ж�)
//	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  // ʹ��ָ����USART2�����ж�
//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART2�����ж�

//	//ʹ�� USART2�� �������
//	USART_Cmd(USART2, ENABLE);                             // USART2ʹ��

//	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
//  USART_ClearFlag(USART2, USART_FLAG_TC);                //�崮��2���ͱ�־
//}






/*
 * ��������fputc
 * ����  ���ض���c�⺯��printf��USART1
 * ����  ����
 * ���  ����
 * ����  ����printf����
 */
//int fputc(int ch, FILE *f)
//{
//	/* ��Printf���ݷ������� */
//	USART_SendData(USART1, (unsigned char) ch);
//	while (!(USART1->SR & USART_FLAG_TXE));
//	
//	return (ch);
//}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

