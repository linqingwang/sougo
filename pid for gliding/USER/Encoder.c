#include "Encoder.h"
#include "stm32f10x.h"
#include "misc.h"
extern u16 En_Counter1;
extern u16 En_Counter2;
extern float velocity;
extern volatile int16_t pulse_num;
u16 pulse_num_flag1;
u16 pulse_num_flag2;

/* 定时器编码器接口模式初始化 */
void TIM2_ENC_Init(void)
{                                                                  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;//定义定时器输入捕获结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);		
	

	// Timer configuration in Encoder mode
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
	TIM_TimeBaseStructure.TIM_Period = ENC_PERIOD; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;//ICx_FILTER;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);


	// Clear all pending interrupts

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	//Reset counter  
	TIM2->CNT = Enc_Mid;

	TIM_Cmd(TIM2, ENABLE); 

}

/* 获取编码器读数 */
uint16_t Get_Enc_Cnt(void)
{
	return TIM_GetCounter(TIM2);
}

// uint16_t Get_velocity(void)
// {
//    En_Counter1=Get_Enc_Cnt();
// 	 pulse_num_flag1=pulse_num;
//    Delay_us(1);
//    En_Counter2=Get_Enc_Cnt();	
// 	 pulse_num_flag2=pulse_num; 
// 	 if(pulse_num_flag1==pulse_num_flag2)
// 	 velocity=(En_Counter2-En_Counter1)*1000.0/38912;
// 	 if(pulse_num_flag1<pulse_num_flag2)
// 	 velocity=(En_Counter2+38911-En_Counter1)*10.0/38912;
// 	 return velocity;	 
// }
