/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：usart1.c
 * 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
 *           打印到PC上的超级终端或串口调试助手。         
 * 实验平台：野火STM32开发板
 * 硬件连接：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/	
#include "usart1.h"
#include "misc.h"

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 


//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;    

	USART_SendData(USART3, (unsigned char) ch);
  while (!(USART3->SR & USART_FLAG_TXE));
  
	return ch;
}
#endif 



/*
 * 函数名：USART1_Config
 * 描述  ：USART1 GPIO 配置,工作模式配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
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


//串口3初始化
void USART3_Config(void)   //初始化 配置USART3
{
	GPIO_InitTypeDef GPIO_InitStructure;	  //串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;	  //串口参数配置结构体变量

	//使能 USART3 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//打开串口复用时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //打开PB端口时钟
  	
	//将USART3 Tx（发送脚）的GPIO配置为推挽复用模式   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				  //选定哪个IO口 现选定PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //设定IO口的输出速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);    				  //初始化GPIOB
	
	//将USART3 Rx（接收脚）的GPIO配置为浮空输入模式														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //选定哪个IO口 现选定PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //初始化GPIOA
	  
	//配置USART3参数
	USART_InitStructure.USART_BaudRate = 115200;	                    //波特率设置115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(USART3, &USART_InitStructure);                       //初始化USART3
	
  //打开发送中断和接收中断(如果需要中断)
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 使能指定的USART3接收中断

	//使能 USART3， 配置完毕
	USART_Cmd(USART3, ENABLE);                             // USART3使能

	//如下语句解决第1个字节无法正确发送出去的问题
  USART_ClearFlag(USART3, USART_FLAG_TC);                //清串口3发送标志
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
	
	//	//串口3中断优先级设置(RS485)
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			   //指定中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	   //抢占优先级0优于抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;             //指定响应优先级别1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	               //打开中断
	NVIC_Init(&NVIC_InitStructure);							       //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  //中断通道：定时器中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级为2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//从优先级为3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能TIM2中断
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


//void USART2_Config(void)   //初始化 配置USART2
//{
//	GPIO_InitTypeDef    GPIO_InitStructure;	   //串口端口配置结构体变量
//	USART_InitTypeDef   USART_InitStructure;   //串口参数配置结构体变量

//	//使能 USART2 时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//打开串口复用时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);   //打开PA端口时钟

//	//将USART2 Tx（发送脚）的GPIO配置为推挽复用模式   
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				  //选定哪个IO口 现选定PA2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //设定IO口的输出速度为50MHz
//	GPIO_Init(GPIOA, &GPIO_InitStructure);    				  //初始化GPIOA

//	//将USART2 Rx（接收脚）的GPIO配置为浮空输入模式														  
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				  //选定哪个IO口 现选定PA3
//	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
//	GPIO_Init(GPIOA, &GPIO_InitStructure);                    //初始化GPIOA
//	  
//	//配置USART2参数
//	USART_InitStructure.USART_BaudRate = 115200;	                    //波特率设置：115200
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
//	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
//	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
//	USART_Init(USART2, &USART_InitStructure);                       //初始化USART2
//	
//	//打开发送中断和接收中断(如果需要中断)
//	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  // 使能指定的USART2发送中断
//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 使能指定的USART2接收中断

//	//使能 USART2， 配置完毕
//	USART_Cmd(USART2, ENABLE);                             // USART2使能

//	//如下语句解决第1个字节无法正确发送出去的问题
//  USART_ClearFlag(USART2, USART_FLAG_TC);                //清串口2发送标志
//}






/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
//int fputc(int ch, FILE *f)
//{
//	/* 将Printf内容发往串口 */
//	USART_SendData(USART1, (unsigned char) ch);
//	while (!(USART1->SR & USART_FLAG_TXE));
//	
//	return (ch);
//}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

