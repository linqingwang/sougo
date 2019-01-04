/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ����ʱ��TIM3������·PWM�������
 *           - PA.06: (TIM3_CH1)
 *           - PA.07: (TIM3_CH2)

 *           TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
 *           TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%

 *       
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "stm32f10x.h"
#include "pwm_output.h"
#include "Encoder.h"
#include "usart1.h"
#include "misc.h"
#include "SysTick.h"
#include "exti.h"
#include "led.h"
#include "Time_test.h"
#include "stdlib.h"
// #include "math.h"
#include "nrf24l01.h"
#include "string.h"
#include "timer.h"
//#include "delay.h"

float a[3],w[3],angle[3],T;
float angle_set=0.00;
extern unsigned char Re_buf[11],temp_buf[11],counter;
extern unsigned char sign;


uint32_t En_Counter1=0;
uint32_t En_Counter2=0;
extern volatile int16_t pulse_num;
u16 TIMCompare1=0;
#define pi 3.14;
static float ka=3.00;
static float kb=3.00;
static float kc=1.00;
static float kd=3.00;
static float head_stable=0.00;
static float tail_stab=0.00;
static float dif_angle=0.00;
float head_angle_2=0.0;
//static float head_stable=0.00;



extern u8 flag;
volatile u32 time; // ms ��ʱ����
volatile double velocity6 = 0;
volatile double velocity7 = 0;
volatile double velocity8 = 0;
volatile double velocity9 = 0;

int OUT5=9;
int OUT4=9;
int OUT3=9;
int OUT2=9;

float Motion_angle1=0.0;
float Motion_angle5=0.0;
float tail_angle=0.0;
float head_angle=0.0;
float head_angle_1=0.01;
float head_angle_bias=0.0;
float head_angle_add=0.0;
float speed=0.0;
int Output1 = 0;
int Output2 = 0;
int Output3 = 0;
int Output4 = 0;
int Output5 = 0;

s8 EncFullFlag = 0;

float A=116736/3;
float t=0;
float dett=0.02;
float f=1.0;
u8 array[32] = {3.0,30.0,0.0,30.0,4.0,50.0,50.0};
u8 ar=0;
u8 ta=0;
u8 aa=9;
int timeflag=0;
static float intialpian=0.0;
float pianhang_angle=0.0;
float inityaw=0;
int yaw=0;

	extern long int	EXTI10_VAL;	//���벶��ֵ
	extern long int	EXTI10_cycle;	//���벶��ֵ
/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */
int main(void)
{
	unsigned char Temp[11];
	SystemInit();
	NVIC_Configuration();//�ж����ȼ�����
	SysTick_Init();
	EXTIX_Init();		//�ⲿ�жϳ�ʼ��
	TIM_PWM_Init(); //�趨��ʱ��2 4ΪPWM
	TIM3_Configuration();//�趨��ʱ��3ΪPWM������20msʱ��
	NRF24L01_Init();//��������
   RCC_Configuration();       //��ʼ��ʱ��
   GPIO_Configuration();      //��ʼ��IO�˿�
   USART1_Config();   //��ʼ�� ����USART1
   USART3_Config();   //��ʼ�� ����USART3 �����ڽ���MPU6050���ݣ�
   TIM3_Int_Init(499,7199);//��Ӧ200ms
   Delay_ms(200);	 
/**********************����while������������ߴ��亯��*************************/
while(1){
	if (sign){sign=0;
		memcpy(Temp,Re_buf,11);
		if(Re_buf[0]==0x55){
			switch (Re_buf[1])
			{
				case 0x51:
				a[0] = ((short)(Temp[3]<<8 | Temp[2]))/32768.0*16;      //X����ٶȣ�16g
                a[1] = ((short)(Temp[5]<<8 | Temp[4]))/32768.0*16;      //Y����ٶ�
                a[2] = ((short)(Temp[7]<<8 | Temp[6]))/32768.0*16;      //Z����ٶ�
                T    = ((short)(Temp[9]<<8 | Temp[8]))/340.0+36.25;      //�¶�	
				case 0x52: //��ʶ������ǽ��ٶȰ�
                  w[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*2000;      //X����ٶ�
                  w[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*2000;      //Y����ٶ�
                  w[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*2000;      //Z����ٶ�
                  T    = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;      //�¶�
                  break;
               case 0x53: //��ʶ������ǽǶȰ�
                  angle[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*180;   //X���ת�ǣ�x �ᣩ
                  angle[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*180;   //Y�ḩ���ǣ�y �ᣩ
                  angle[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*180;   //Z��ƫ���ǣ�z �ᣩ
                  T        = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;   //�¶�
                  break;
							 
               default:  break;
			}
// 			//�㷨���+��̬����		
		angle_set=3.0*(EXTI10_cycle-2.0);//��Ҫ����ƫ�ڵ����߿��Ƽ���½����
//		if(EXTI10_cycle<1)
//			pianhang_angle=0;
//		else
			if(EXTI10_cycle>=0 && EXTI10_cycle<7)
			pianhang_angle=kd*EXTI10_cycle;//��һ�����˶���Χ������ƫת
		
		//head_angle=angle[1];
		//head_angle_bias=angle[1]-angle_set;//ͷ��������Ŀ���趨ֵ֮���ƫ��
		

			ta=NRF24L01_RxPacket(array);						
						ka=array[1]/10.0;	
						kc=array[2]/10.0;	
						kb=array[3]/10.0;	
						kd=array[4]-10.0;	
						dif_angle=-(array[5]-50.0);
						intialpian=array[6]-60.0;
      //new added by Jack!;
			head_angle=angle[1]+head_stable-angle_set;
			head_angle_add=head_angle-(head_angle_1+head_angle_2)/2;
			if((head_angle>-5)&&(head_angle<5)&&(head_angle_add<1)&&(head_angle_add>-1))
				head_angle_add=0;
			if((head_angle>=-10)&&(head_angle<10))
				Motion_angle1=ka*head_angle-kc*head_angle_add;
			if((head_angle>-25)&&(head_angle<-10))//�����ڡ�6-15�Ƕȷ�Χ��̧ͷ��
				Motion_angle1=kb*(head_angle+10)-10*ka-kc*head_angle_add;//�ı�β�͸�������������ͷ��
			if((head_angle>=10)&&(head_angle<25))//�����ڡ�6-15�Ƕȷ�Χ�ڵ�ͷ��
				Motion_angle1=kb*(head_angle-10)+10*ka-kc*head_angle_add;//�ı�β�͸�������������ͷ��
			
			Motion_angle5=intialpian+pianhang_angle; 			
			Output1=9.1469*(Motion_angle1+dif_angle-18)+1399.43;
			Output5=-11.968* (Motion_angle5+8.0)+1351.32;
			TIM_SetCompare2(TIM4,Output1);//second pin the num1 motion
			TIM_SetCompare3(TIM2,Output5);//ǰ֫�˶�����
			head_angle_2=head_angle_1;
			head_angle_1=head_angle;
			}
		}
	}
}		
//��������ģ��		


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

