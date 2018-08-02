/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��pwm_output.c
 * ����    ��         
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�---------------------
 *          |  PA.06: (TIM3_CH1)  |
 *          |  PA.07: (TIM3_CH2)  |
 *      	|  PB.00: (TIM3_CH3)  | 
 *    		|  PB.01: (TIM3_CH4)  |
 *           ---------------------    			
 * ��汾  ��ST3.5.0
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "pwm_output.h"
/*
 * ��������TIM2_GPIO_Config
 * ����  ������TIM3�������PWMʱ�õ���I/O
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM2_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	//PCLK1����2��Ƶ����ΪTIM3��ʱ��Դ����72MHz
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

	/* GPIOA and GPIOB clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 

	/*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/*
 * ��������TIM3_Mode_Config
 * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
  u16 CCR1_Val =100;        
	u16 CCR2_Val = 100;
  u16 CCR3_Val =100;        
	u16 CCR4_Val = 100;
static void TIM2_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */



/* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 72 KHz.
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 20000-1;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = (71);	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);	 //ʹ��ͨ��1

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC2Init(TIM2, &TIM_OCInitStructure);	  //ʹ��ͨ��2

  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	  //����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);	  //ʹ��ͨ��3

  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC4Init(TIM2, &TIM_OCInitStructure);	  //ʹ��ͨ��4

  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM2, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM2, ENABLE);                   //ʹ�ܶ�ʱ��3	
}
/*
 * ��������TIM3_GPIO_Config
 * ����  ������TIM3�������PWMʱ�õ���I/O
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
// static void TIM3_GPIO_Config(void) 
// {
//   GPIO_InitTypeDef GPIO_InitStructure;

// 	/* TIM3 clock enable */
// 	//PCLK1����2��Ƶ����ΪTIM3��ʱ��Դ����72MHz
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

//   /* GPIOA and GPIOB clock enable */
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 

// 	 /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
//   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

// // 	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
// //   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    // �����������
// //   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	
//   /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
//   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
// // 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

//   GPIO_Init(GPIOB, &GPIO_InitStructure);

// }

// /*
//  * ��������TIM3_Mode_Config
//  * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
//  * ����  ����
//  * ���  ����
//  * ����  ���ڲ�����
//  */

// static void TIM3_Mode_Config(void)
// {
// 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// 	TIM_OCInitTypeDef  TIM_OCInitStructure;

// 	/* PWM�źŵ�ƽ����ֵ */



// /* -----------------------------------------------------------------------
//     TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
//     TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
//     TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
//     TIM3 Frequency = 72 KHz.
//     TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
//     TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
//   ----------------------------------------------------------------------- */

//   /* Time base configuration */		 
//   TIM_TimeBaseStructure.TIM_Period = 20000-1;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
//   TIM_TimeBaseStructure.TIM_Prescaler = (71);	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
//   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
//   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

//   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

//   /* PWM1 Mode configuration: Channel1 */
//   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
//   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
//   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

//   TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1

//   TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   /* PWM1 Mode configuration: Channel2 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

//   TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��2

//   TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   /* PWM1 Mode configuration: Channel3 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	  //����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

//   TIM_OC3Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��3

//   TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
// 	
// 	  /* PWM1 Mode configuration: Channel4 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

//   TIM_OC4Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��4

//   TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

//   /* TIM3 enable counter */
//   TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3	
// }



static void TIM4_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM4 clock enable */
	//PCLK1����2��Ƶ����ΪTIM4��ʱ��Դ����72MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 

	 /*GPIOA Configuration: TIM4 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/*
 * ��������TIM4_Mode_Config
 * ����  ������TIM4�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */

static void TIM4_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */



/* -----------------------------------------------------------------------
    TIM4 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM4CLK = 72 MHz, Prescaler = 0x0, TIM4 counter clock = 72 MHz
    TIM4 ARR Register = 999 => TIM4 Frequency = TIM4 counter clock/(ARR + 1)
    TIM4 Frequency = 72 KHz.
    TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 50%
    TIM4 Channel2 duty cycle = (TIM4_CCR2/ TIM4_ARR)* 100 = 37.5%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 20000-1;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = (71);	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);	 //ʹ��ͨ��1

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC2Init(TIM4, &TIM_OCInitStructure);	  //ʹ��ͨ��2

  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	  //����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC3Init(TIM4, &TIM_OCInitStructure);	  //ʹ��ͨ��3

  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC4Init(TIM4, &TIM_OCInitStructure);	  //ʹ��ͨ��4

  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM4, ENABLE);			 // ʹ��TIM4���ؼĴ���ARR

  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);                   //ʹ�ܶ�ʱ��3	
}


/*
 * ��������TIM3_PWM_Init
 * ����  ��TIM3 ���PWM�źų�ʼ����ֻҪ�����������
 *         TIM3���ĸ�ͨ���ͻ���PWM�ź����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIM_PWM_Init(void)
{
	TIM2_GPIO_Config();
	TIM2_Mode_Config();	
// 	TIM3_GPIO_Config();
// 	TIM3_Mode_Config();	
	TIM4_GPIO_Config();
	TIM4_Mode_Config();	
}

void Step_M_Rps(float rps)
{
	float xifen=16;
  uint16_t tim3_prescale=3;
	uint16_t ARRtemp=1000;
	
	ARRtemp=(uint16_t)(360000.0/xifen/(float)tim3_prescale/rps); // 360000.0=72M/(360degree/1.8degree)
	
 /* Set the Autoreload value */
  TIM3->ARR =	ARRtemp;
	
	/* Set the Capture Compare Register value */
	TIM3->CCR1 = ARRtemp/2;
  TIM3->CCR2 = ARRtemp/2;
	 
  /* Set the Prescaler value */
  TIM3->PSC = tim3_prescale-1;	
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
