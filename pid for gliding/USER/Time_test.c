/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：Time_test.c
 * 描述    ：TIM2 1ms 定时应用函数库
 *          
 * 实验平台：野火STM32开发板
 * 硬件连接：----------------------
 *          |					  |
 *          |	        无     	  |
 *          |					  |
 *           ----------------------
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "Time_test.h"


/*
 * 函数名：TIM2_NVIC_Configuration
 * 描述  ：TIM2中断优先级配置
 * 输入  ：无
 * 输出  ：无	
 */
// void TIM4_NVIC_Configuration(void)
// {
//     NVIC_InitTypeDef NVIC_InitStructure; 
//     
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  		//中断第零组											
//     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  //中断通道：定时器中断
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级为0
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//从优先级为3
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能TIM2中断
//     NVIC_Init(&NVIC_InitStructure);
// }


/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM3_Configuration(void)
{
	//基础设置
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
    TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.TIM_Period=20000;		 								/* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    /* 时钟预分频数 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM3, ENABLE);																		/* 开启时钟 */
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);		/*先关闭等待使用*/    
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

//void Timer2_Init(u16 arr,u16 psc)  //定时器2初始化函数
//{
//	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;  //定义配置定时器的结构体变量
//	
//	//定时器2库函数初始化部分：
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能TIM2时钟
//	TIM_DeInit(TIM2);	            //将外设TIMx寄存器重设为缺省值 
//	TIM_InternalClockConfig(TIM2);  //定时器时钟源，选择内部时钟
//	/*基础设置*/
//	TIM_TimeBaseStructure.TIM_Period = arr - 1;                     //设定计数器自动重装值  
//	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;    	            //预分频,此值+1为分频的除数
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  	                //设置时钟分割  采样分频
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
//	TIM2->CR1  &= ~(1<<7);   //自动重装ARR里的值没有缓冲,随时更新数据随时有效。
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);					//初始化

//	TIM_Cmd(TIM2, ENABLE);						 //使能定时器2，开始定时
//	//TIM_Cmd(TIM2, DISABLE);						 //暂时不启动使能定时器2
//	TIM_PrescalerConfig(TIM2, psc - 1, TIM_PSCReloadMode_Immediate);//让定时器预分频值立即装入
//	TIM_ClearFlag(TIM2,TIM_FLAG_Update);     //清除中断标志
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //使能定时器中断
//}

//void Timer3_Init(u16 arr,u16 psc)  //定时器3初始化函数
//{
//	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;  //定义配置定时器的结构体变量
//	
//	//定时器3库函数初始化部分：
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //使能TIM3时钟
//	TIM_DeInit(TIM3);	            //将外设TIMx寄存器重设为缺省值 
//	TIM_InternalClockConfig(TIM3);  //定时器时钟源，选择内部时钟
//	/*基础设置*/
//	TIM_TimeBaseStructure.TIM_Period = arr - 1;                     //设定计数器自动重装值  
//	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;    	            //预分频,此值+1为分频的除数
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  	                //设置时钟分割  采样分频
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
//	TIM3->CR1  &= ~(1<<7);   //自动重装ARR里的值没有缓冲,随时更新数据随时有效。
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);					//初始化

//	TIM_Cmd(TIM3, ENABLE);						 //使能定时器3，开始定时
//	//TIM_Cmd(TIM3, DISABLE);						 //暂时不启动使能定时器3
//	TIM_PrescalerConfig(TIM3, psc - 1, TIM_PSCReloadMode_Immediate);//让定时器预分频值立即装入
//	TIM_ClearFlag(TIM3,TIM_FLAG_Update);     //清除中断标志
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //使能定时器中断
//}

//void Timer7_Init(u16 arr,u16 psc)  //定时器7初始化函数
//{
//	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;  //定义配置定时器的结构体变量
//	
//	//定时器7库函数初始化部分：
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //使能TIM7时钟
//	TIM_DeInit(TIM7);	            //将外设TIMx寄存器重设为缺省值 
//	TIM_InternalClockConfig(TIM7);  //定时器时钟源，选择内部时钟
//	/*基础设置*/
//	TIM_TimeBaseStructure.TIM_Period = arr - 1;                     //设定计数器自动重装值  
//	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;    	            //预分频,此值+1为分频的除数
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  	                //设置时钟分割  采样分频
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
//	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);					//初始化

//	TIM_Cmd(TIM7, ENABLE);						 //使能定时器7，开始定时
//	TIM_PrescalerConfig(TIM7, psc - 1, TIM_PSCReloadMode_Immediate);//让定时器预分频值立即装入
//	TIM_ClearFlag(TIM7,TIM_FLAG_Update);     //清除中断标志
//	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //使能定时器中断
//}



