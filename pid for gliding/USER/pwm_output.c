/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：pwm_output.c
 * 描述    ：         
 * 实验平台：野火STM32开发板
 * 硬件连接：---------------------
 *          |  PA.06: (TIM3_CH1)  |
 *          |  PA.07: (TIM3_CH2)  |
 *      	|  PB.00: (TIM3_CH3)  | 
 *    		|  PB.01: (TIM3_CH4)  |
 *           ---------------------    			
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "pwm_output.h"
/*
 * 函数名：TIM2_GPIO_Config
 * 描述  ：配置TIM3复用输出PWM时用到的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void TIM2_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	//PCLK1经过2倍频后作为TIM3的时钟源等于72MHz
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

	/* GPIOA and GPIOB clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 

	/*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/*
 * 函数名：TIM3_Mode_Config
 * 描述  ：配置TIM3输出的PWM信号的模式，如周期、极性、占空比
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
  u16 CCR1_Val =100;        
	u16 CCR2_Val = 100;
  u16 CCR3_Val =100;        
	u16 CCR4_Val = 100;
static void TIM2_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */



/* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 72 KHz.
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 20000-1;       //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = (71);	    //设置预分频：不预分频，即为72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);	 //使能通道1

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM

  TIM_OC2Init(TIM2, &TIM_OCInitStructure);	  //使能通道2

  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	  //设置通道3的电平跳变值，输出另外一个占空比的PWM

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);	  //使能通道3

  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //设置通道4的电平跳变值，输出另外一个占空比的PWM

  TIM_OC4Init(TIM2, &TIM_OCInitStructure);	  //使能通道4

  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM2, ENABLE);			 // 使能TIM3重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM2, ENABLE);                   //使能定时器3	
}
/*
 * 函数名：TIM3_GPIO_Config
 * 描述  ：配置TIM3复用输出PWM时用到的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
// static void TIM3_GPIO_Config(void) 
// {
//   GPIO_InitTypeDef GPIO_InitStructure;

// 	/* TIM3 clock enable */
// 	//PCLK1经过2倍频后作为TIM3的时钟源等于72MHz
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

//   /* GPIOA and GPIOB clock enable */
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 

// 	 /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
//   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

// // 	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
// //   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    // 复用推挽输出
// //   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	
//   /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
//   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
// // 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

//   GPIO_Init(GPIOB, &GPIO_InitStructure);

// }

// /*
//  * 函数名：TIM3_Mode_Config
//  * 描述  ：配置TIM3输出的PWM信号的模式，如周期、极性、占空比
//  * 输入  ：无
//  * 输出  ：无
//  * 调用  ：内部调用
//  */

// static void TIM3_Mode_Config(void)
// {
// 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// 	TIM_OCInitTypeDef  TIM_OCInitStructure;

// 	/* PWM信号电平跳变值 */



// /* -----------------------------------------------------------------------
//     TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
//     TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
//     TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
//     TIM3 Frequency = 72 KHz.
//     TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
//     TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
//   ----------------------------------------------------------------------- */

//   /* Time base configuration */		 
//   TIM_TimeBaseStructure.TIM_Period = 20000-1;       //当定时器从0计数到999，即为1000次，为一个定时周期
//   TIM_TimeBaseStructure.TIM_Prescaler = (71);	    //设置预分频：不预分频，即为72MHz
//   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频
//   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

//   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

//   /* PWM1 Mode configuration: Channel1 */
//   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
//   TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
//   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平

//   TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1

//   TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   /* PWM1 Mode configuration: Channel2 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM

//   TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //使能通道2

//   TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   /* PWM1 Mode configuration: Channel3 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	  //设置通道3的电平跳变值，输出另外一个占空比的PWM

//   TIM_OC3Init(TIM3, &TIM_OCInitStructure);	  //使能通道3

//   TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
// 	
// 	  /* PWM1 Mode configuration: Channel4 */
//   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //设置通道4的电平跳变值，输出另外一个占空比的PWM

//   TIM_OC4Init(TIM3, &TIM_OCInitStructure);	  //使能通道4

//   TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

//   TIM_ARRPreloadConfig(TIM3, ENABLE);			 // 使能TIM3重载寄存器ARR

//   /* TIM3 enable counter */
//   TIM_Cmd(TIM3, ENABLE);                   //使能定时器3	
// }



static void TIM4_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM4 clock enable */
	//PCLK1经过2倍频后作为TIM4的时钟源等于72MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 

	 /*GPIOA Configuration: TIM4 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/*
 * 函数名：TIM4_Mode_Config
 * 描述  ：配置TIM4输出的PWM信号的模式，如周期、极性、占空比
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */

static void TIM4_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */



/* -----------------------------------------------------------------------
    TIM4 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM4CLK = 72 MHz, Prescaler = 0x0, TIM4 counter clock = 72 MHz
    TIM4 ARR Register = 999 => TIM4 Frequency = TIM4 counter clock/(ARR + 1)
    TIM4 Frequency = 72 KHz.
    TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 50%
    TIM4 Channel2 duty cycle = (TIM4_CCR2/ TIM4_ARR)* 100 = 37.5%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 20000-1;       //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = (71);	    //设置预分频：不预分频，即为72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);	 //使能通道1

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM

  TIM_OC2Init(TIM4, &TIM_OCInitStructure);	  //使能通道2

  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	  //设置通道3的电平跳变值，输出另外一个占空比的PWM

  TIM_OC3Init(TIM4, &TIM_OCInitStructure);	  //使能通道3

  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //设置通道4的电平跳变值，输出另外一个占空比的PWM

  TIM_OC4Init(TIM4, &TIM_OCInitStructure);	  //使能通道4

  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM4, ENABLE);			 // 使能TIM4重载寄存器ARR

  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);                   //使能定时器3	
}


/*
 * 函数名：TIM3_PWM_Init
 * 描述  ：TIM3 输出PWM信号初始化，只要调用这个函数
 *         TIM3的四个通道就会有PWM信号输出
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
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
