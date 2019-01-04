/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "Encoder.h"
#include "led.h"
#include "SysTick.h"
#include "stdlib.h"
#include "sys.h"
#include "string.h"
#include "usart1.h"
volatile int16_t cyc_num=0;
volatile int16_t pulse_num;
volatile int16_t num=0;
extern u16 En_Counter;
extern void TimingDelay_Decrement(void);
extern void Pulse_up(void);
extern u16 En_Counter1;
extern u16 En_Counter2;
u16 En_Counter0;
extern float velocity;
#define pi 3.14;
void Get_velocity(void);
extern volatile u32 time;
extern s8 EncFullFlag;
extern s32 target_Position;
extern volatile s32 target_Position_BL ;
extern int timeflag;
extern float A;
extern float t;
extern float dett;
extern float f;
/*�¼Ӵ����շ�����*/
extern u16 CCR1_Val;        
extern u16 CCR2_Val;

unsigned char Re_buf[11],temp_buf[11],counter=0;
unsigned char sign,he;

static unsigned char Temp[11];

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/*ԭ�г��򣬲��Ӵ��������*/
// void USART1_IRQHandler(void)
// {
// 	u8 c;
// 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
// 	{ 	
// 	    c=USART1->DR;
// 	  	printf("%c",c);    //�����ܵ�������ֱ�ӷ��ش�ӡ
// 	} 
// 	 
// }


/*�����շ����*/
  u8 flag=0;
	u8 control[4]={0};
	u8 number=0;
void USART1_IRQHandler(void)
{
	u8 c;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
		  USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	    c=USART1->DR;
	  	//printf("%c",c);    //�����ܵ�������ֱ�ӷ��ش�ӡ
		  control[number]=c;
		  number++;
		  if(number==4)
			{
				flag=1;
				number=0;
			}
	} 
	 
}

void USART3_IRQHandler(void)		   //����3ȫ���жϷ�����
{
//    u8 m; 
   if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж���Ч,���������ݼĴ�����
   {
//   
//    m=USART_ReceiveData(USART3);
//		printf("%c",m);
      Temp[counter] = USART_ReceiveData(USART3);   //��������
      //�������ĳ���
      //if(counter == 0 && Re_buf[0] != 0x55) return;      //�� 0 �����ݲ���֡ͷ������
	  if(counter == 0 && Temp[0] != 0x55) return;      //�� 0 �����ݲ���֡ͷ������
      counter++; 
      if(counter==11) //���յ� 11 ������
      { 
         memcpy(Re_buf,Temp,11);
         counter=0; //���¸�ֵ��׼����һ֡���ݵĽ���
         sign=1;
      }    
    } 
	  USART_ClearITPendingBit(USART3,USART_IT_RXNE);
 }


extern u8 array[32];
extern u8 ta;
extern u8 aa;
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET) //ȷ���Ƿ������EXTI Line�ж�
  { 
//  	a=NRF24L01_RxPacket(array);
	aa=01;	
// 	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) == 1)
// 		{
// 		  num++;
// 		}
// 	else
// 	num--;		
//	En_Counter=Get_Enc_Cnt();
//	TIM2->CNT = Enc_Mid;
//	cyc_num++;
//	if (cyc_num==3) 
//	Pulse_up();
//	TIM_Cmd(TIM3,DISABLE);  		 
//	printf("%d ",cyc_num);
//	printf("%d ",pulse_num);
//	printf("%d ",En_Counter);	
//	Pulse_up();
//	if (pulse_num==1)
//	printf("%d",1);	
//	else 
//	printf("%d",0);
//	pulse_num=0;
//  ͨ��Z����Ƶ��ת�������תnȦֹͣת��				
//	printf("%d ",En_Counter);		
    EXTI_ClearITPendingBit(EXTI_Line5);     //����жϱ�־λ
  }
	
		
}

//void EXTI15_10_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(EXTI_Line13) != RESET) //ȷ���Ƿ������EXTI Line�ж�
//  {
//				
//    //pulse_num++;	
//		//ͨ��TIM2����AB�����������Ƶ��ת��
//		//if (pulse_num==2000)  //2תֹͣ
//		//TIM_Cmd(TIM3,DISABLE);//�رն�ʱ��3
//		EXTI_ClearITPendingBit(EXTI_Line13);     //����жϱ�־λ
//  }  
//}

void TIM2_IRQHandler(void)//��������
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
// 		GPIO_WriteBit(GPIOC, GPIO_Pin_3, 
// 			(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_3))));
		if (TIM2->CNT > 20000)
			EncFullFlag--;
		else if (TIM2->CNT < 10000)
			EncFullFlag++;		
//	  pulse_num++;
//	  printf("%d  ",EncFullFlag);
//    printf("%.2f\r\n",velocity);
		
//	printf("%d ",velocity);
// 		if(pulse_num==512)
// 		LED1(ON);		
  //	if (TIM8->CNT <1)
	//	num++;
  // 	GPIO_WriteBit(GPIOC, GPIO_Pin_3, 
  // 	(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_3))));
	//  if (cyc_num==2)
	//	Delay_us(1000);
	//	GPIO_SetBits(GPIOC, GPIO_Pin_3);
	//  Pulse_up();

	}
}


//void TIM3_IRQHandler(void)//���ڲ���
//{
//	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) //����Ƿ���������жϣ�TIM_IT_Update�����ж�Դ
//	{	
//		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);    //����жϱ�־λ
////  	time++;
//// 		if ( time ==500 )
//// 		target_Position_BL = 50000;
//// 		if ( time ==1000 )
//// 		target_Position_BL = 100000;
//// 		if ( time ==1500 )
//// 		target_Position_BL =50000;
//// 		if ( time ==2000 )
//// 		target_Position_BL =0;
//		
//// 	    t=t+dett; 
////   		target_Position_BL=(s32)(A*sin(2*3.14*f*t));
//// 		
//		  timeflag=1;
//		
//		
//		
//		
//		
//		
//		
//// //�ٶȲɼ�����
//// 		En_Counter0=Get_Enc_Cnt();    //��������
//// 		En_Counter2=En_Counter0+EncFullFlag*38912;//�ɼ��յ�
//// 		velocity=(En_Counter2-En_Counter1)*0.025699;//1000/38912=0.025699
//// 		En_Counter1=En_Counter2;//���ɼ��յ��Ϊ��ʼ�ص�
////	printf("%.2f\r\n",velocity);
//		
//// 		if ( time == 1000 ) /* 1s ʱ�䵽 */
////     {
////       time = 0;
//// 			/* LED1 ȡ�� */      
//// 			GPIO_WriteBit(GPIOC, GPIO_Pin_3, 
//// 		               (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_3)))); 
////		}
//	}		 	
//}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
