/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��Time_test.c
 * ����    ��TIM2 1ms ��ʱӦ�ú�����
 *          
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�----------------------
 *          |					  |
 *          |	        ��     	  |
 *          |					  |
 *           ----------------------
 * ��汾  ��ST3.5.0
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "Time_test.h"


/*
 * ��������TIM2_NVIC_Configuration
 * ����  ��TIM2�ж����ȼ�����
 * ����  ����
 * ���  ����	
 */
// void TIM4_NVIC_Configuration(void)
// {
//     NVIC_InitTypeDef NVIC_InitStructure; 
//     
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  		//�жϵ�����											
//     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  //�ж�ͨ������ʱ���ж�
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�Ϊ0
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//�����ȼ�Ϊ3
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ��TIM2�ж�
//     NVIC_Init(&NVIC_InitStructure);
// }


/*TIM_Period--1000   TIM_Prescaler--71 -->�ж�����Ϊ1ms*/
void TIM3_Configuration(void)
{
	//��������
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
    TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.TIM_Period=20000;		 								/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    /* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    		/* �������жϱ�־ */
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM3, ENABLE);																		/* ����ʱ�� */
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);		/*�ȹرյȴ�ʹ��*/    
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

//void Timer2_Init(u16 arr,u16 psc)  //��ʱ��2��ʼ������
//{
//	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;  //�������ö�ʱ���Ľṹ�����
//	
//	//��ʱ��2�⺯����ʼ�����֣�
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʹ��TIM2ʱ��
//	TIM_DeInit(TIM2);	            //������TIMx�Ĵ�������Ϊȱʡֵ 
//	TIM_InternalClockConfig(TIM2);  //��ʱ��ʱ��Դ��ѡ���ڲ�ʱ��
//	/*��������*/
//	TIM_TimeBaseStructure.TIM_Period = arr - 1;                     //�趨�������Զ���װֵ  
//	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;    	            //Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  	                //����ʱ�ӷָ�  ������Ƶ
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
//	TIM2->CR1  &= ~(1<<7);   //�Զ���װARR���ֵû�л���,��ʱ����������ʱ��Ч��
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);					//��ʼ��

//	TIM_Cmd(TIM2, ENABLE);						 //ʹ�ܶ�ʱ��2����ʼ��ʱ
//	//TIM_Cmd(TIM2, DISABLE);						 //��ʱ������ʹ�ܶ�ʱ��2
//	TIM_PrescalerConfig(TIM2, psc - 1, TIM_PSCReloadMode_Immediate);//�ö�ʱ��Ԥ��Ƶֵ����װ��
//	TIM_ClearFlag(TIM2,TIM_FLAG_Update);     //����жϱ�־
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //ʹ�ܶ�ʱ���ж�
//}

//void Timer3_Init(u16 arr,u16 psc)  //��ʱ��3��ʼ������
//{
//	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;  //�������ö�ʱ���Ľṹ�����
//	
//	//��ʱ��3�⺯����ʼ�����֣�
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʹ��TIM3ʱ��
//	TIM_DeInit(TIM3);	            //������TIMx�Ĵ�������Ϊȱʡֵ 
//	TIM_InternalClockConfig(TIM3);  //��ʱ��ʱ��Դ��ѡ���ڲ�ʱ��
//	/*��������*/
//	TIM_TimeBaseStructure.TIM_Period = arr - 1;                     //�趨�������Զ���װֵ  
//	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;    	            //Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  	                //����ʱ�ӷָ�  ������Ƶ
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
//	TIM3->CR1  &= ~(1<<7);   //�Զ���װARR���ֵû�л���,��ʱ����������ʱ��Ч��
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);					//��ʼ��

//	TIM_Cmd(TIM3, ENABLE);						 //ʹ�ܶ�ʱ��3����ʼ��ʱ
//	//TIM_Cmd(TIM3, DISABLE);						 //��ʱ������ʹ�ܶ�ʱ��3
//	TIM_PrescalerConfig(TIM3, psc - 1, TIM_PSCReloadMode_Immediate);//�ö�ʱ��Ԥ��Ƶֵ����װ��
//	TIM_ClearFlag(TIM3,TIM_FLAG_Update);     //����жϱ�־
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //ʹ�ܶ�ʱ���ж�
//}

//void Timer7_Init(u16 arr,u16 psc)  //��ʱ��7��ʼ������
//{
//	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;  //�������ö�ʱ���Ľṹ�����
//	
//	//��ʱ��7�⺯����ʼ�����֣�
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //ʹ��TIM7ʱ��
//	TIM_DeInit(TIM7);	            //������TIMx�Ĵ�������Ϊȱʡֵ 
//	TIM_InternalClockConfig(TIM7);  //��ʱ��ʱ��Դ��ѡ���ڲ�ʱ��
//	/*��������*/
//	TIM_TimeBaseStructure.TIM_Period = arr - 1;                     //�趨�������Զ���װֵ  
//	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;    	            //Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  	                //����ʱ�ӷָ�  ������Ƶ
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
//	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);					//��ʼ��

//	TIM_Cmd(TIM7, ENABLE);						 //ʹ�ܶ�ʱ��7����ʼ��ʱ
//	TIM_PrescalerConfig(TIM7, psc - 1, TIM_PSCReloadMode_Immediate);//�ö�ʱ��Ԥ��Ƶֵ����װ��
//	TIM_ClearFlag(TIM7,TIM_FLAG_Update);     //����жϱ�־
//	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //ʹ�ܶ�ʱ���ж�
//}



