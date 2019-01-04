#include "nrf24l01.h"
#include "stm32f10x_spi.h"

// const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
// const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; //发送地址
extern u8 tmp_buf[1];
u8 Transmit[1] = {START_TRANS};
u8 Trans_Status = 0;

/** NRF24L01 GPIO配置 */
static void NRF_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;		// SCK MOSI 推挽复用输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;					// MISO 上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;					// CSN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;					// CE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;					// IRQ 上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	NRF_CE = 0;
	NRF_CSN = 1;
}

/** NRF24L01 外部中断配置 */
static void NRF_Exti_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
	
	  EXTI_InitStructure.EXTI_Line = EXTI_Line5;				
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 	//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);
}

/** NRF24L01 SPI参数配置 */
static void SPI_Config(SPI_TypeDef* SPIx)
{
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRC值计算的多项式
	SPI_Init(SPIx, &SPI_InitStructure);  								//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	SPI_Cmd(SPIx, ENABLE);
}

static void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); //判断有效性
	SPI1->CR1 &= 0XFFC7;											//位3-5清零，用来设置波特率
	SPI1->CR1 |= SPI_BaudRatePrescaler;	    						//设置SPI1速度 
	SPI_Cmd(SPIx,ENABLE); 				   							//使能SPI1
} 

/**
  * @brief  SPI1读、写一个字节
  * @param  TxData：要写入的字节
  * @retval 读取到的字节
  */
static u8 NRF_SPI1_ReadWriteByte(u8 TxData)
{		 			 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){} //等待发送区空  	
	SPI_I2S_SendData(SPI1, TxData); 								 //通过外设SPIx发送一个byte数据		
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}//等待接收完一个byte   
	return SPI_I2S_ReceiveData(SPI1);   							 //返回通过SPIx最近接收的数据			    
}



/**
  * @brief  SPI写寄存器
  * @param  reg：指定寄存器地址
  * @param  value：要写入的值
  * @retval 此次读到的状态寄存器值
  */
static u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
	NRF_CSN = 0;    					 //使能SPI传输
	status = NRF_SPI1_ReadWriteByte(reg);//发送寄存器号 
	NRF_SPI1_ReadWriteByte(value);       //写入寄存器的值
	NRF_CSN = 1;      					 //禁止SPI传输	   
	return(status);       				 //返回状态值
}

/**
  * @brief  SPI读寄存器
  * @param  reg：指定寄存器地址
  * @retval 读取到的值
  */
static u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
	NRF_CSN = 0;       						//使能SPI传输		
	NRF_SPI1_ReadWriteByte(reg);   			//发送寄存器号
	reg_val = NRF_SPI1_ReadWriteByte(NOP); //读取寄存器内容
	NRF_CSN = 1;         					//禁止SPI传输		    
	return(reg_val);           				//返回寄存器中的值
}	

/**
  * @brief  SPI在指定位置读出指定长度的数据
  * @param  reg：指定寄存器地址
  * @param  *puf：存储读取数据的数据指针
  * @param  len：数据长度
  * @retval 此次读到的状态寄存器值
  */ 
static u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
	NRF_CSN = 0;           	//使能SPI传输
	status = NRF_SPI1_ReadWriteByte(reg);			//发送寄存器值(位置),并读取状态值   	   
	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
		pBuf[u8_ctr] = NRF_SPI1_ReadWriteByte(0XFF);//读出数据
	NRF_CSN = 1;       			//关闭SPI传输
	return status;        
}

/**
  * @brief  SPI在指定位置写入指定长度的数据
  * @param  reg：指定寄存器地址
  * @param  *puf：存储要写入数据的数据指针
  * @param  len：数据长度
  * @retval 此次读到的状态寄存器值
  */
static u8 NRF24L01_Write_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	    
	NRF_CSN = 0;       //使能SPI传输
	status = NRF_SPI1_ReadWriteByte(reg);	//发送寄存器值(位置),并读取状态值
	for(u8_ctr=0; u8_ctr<len; u8_ctr++)
		NRF_SPI1_ReadWriteByte(*pBuf++); 	//写入数据	 
	NRF_CSN = 1;       	//关闭SPI传输
	return status;          				//返回读到的状态值
}

/**
  * @brief  读取NRF24L01 STATUS寄存器状态
  * @param  TxData：要写入的字节
  * @retval STATUS寄存器的值
  */
static u8 NRF_GetStatus(void)
{
	u8 status;
	status = NRF24L01_Read_Reg(STATUS);
	return status;
}

/**
  * @brief  检测NRF24L01是否存在
  * @param  无
  * @retval 1：不存在
  * @retval 0：存在
  */
static u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	SPI_SetSpeed(SPI1,SPI_BaudRatePrescaler_8); 	//spi速度为10.5Mhz（24L01的最大SPI时钟为10Mhz）   	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); 				//读出写入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;								//检测24L01错误	
	return 0;		 								//检测到24L01
}	

/**
  * @brief  初始化NRF24L01到TX模式，发送模式
  * @param  无
  * @retval 无
  * @note   设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
  *         PWR_UP,CRC使能，CE为高大于10us,则启动发送
  */	 
static void NRF24L01_TX_Mode(void)
{														 
	NRF_CE = 0;	    
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,0);       //设置RF通道为40
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	NRF_CE = 1;					  					  //CE为高,10us后启动发送
}

/**
  * @brief  初始化NRF24L01到RX模式 接收模式
  * @param  无
  * @retval 无
  * @note   设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
  *         当CE变高后,即进入RX模式,并可以接收数据了
  */		   
static void NRF24L01_RX_Mode(void)
{
	NRF_CE = 0;	  
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);  //选择通道0的有效数据宽度 
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	
	NRF24L01_Write_Reg(FLUSH_RX,0xff);									//清除RX FIFO寄存器   
  
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    			//使能通道0的自动应答    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);			//使能通道0的接收地址  	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,0);	     			//设置RF通信频率		  	    
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0F);			//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);				//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
	NRF_CE = 1; 												//CE为高,进入接收模式 
}

/**
  * @brief  启动NRF24L01发送一次数据
  * @param  txbuf:待发送数据首地址
  * @retval MAX_TX：发送失败，达到最大重发次数
  * @retval TX_OK：发送完成
  * @retval 0xff：其他原因发送失败
  */
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	SPI_SetSpeed(SPI1,SPI_BaudRatePrescaler_8);			  //spi速度为10.5Mhz（24L01的最大SPI时钟为10Mhz）   
	NRF_CE = 0;
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH); //写数据到TX BUF  
 	NRF_CE = 1;						  					  //启动发送	   
	while (NRF_IRQ != 0);		 						  //等待发送完成
	sta=NRF24L01_Read_Reg(STATUS);  					  //读取状态寄存器的值	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 		  //清除TX_DS或MAX_RT中断标志
	if (sta&MAX_TX)										  //达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);				  //清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if (sta&TX_OK)									      //发送完成
		return TX_OK;	
	return 0xff;										  //其他原因发送失败
}

/**
  * @brief  启动NRF24L01接收一次数据
  * @param  rxbuf:存储待接收数据首地址
  * @retval 1：接收失败，没有接收到任何数据
  * @retval 0：接收完成
  */
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI_SetSpeed(SPI1,SPI_BaudRatePrescaler_8); 	//spi速度为10.5Mhz（24L01的最大SPI时钟为10Mhz）   
	sta = NRF24L01_Read_Reg(STATUS);  				//读取状态寄存器的值    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 	//清除TX_DS或MAX_RT中断标志
	if (sta&RX_OK)									//接收到数据
	{ 
		NRF_CE = 0;
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xff);					//清除RX FIFO寄存器 
		NRF_CE = 1;
// 		delay_150us(); 
		return 0; 
	}	   
	return 1;												//没收到任何数据
}

/** NRF24L01 初始化配置 */
void NRF24L01_Init(void)
{
	u8 i = 2;
	NRF_GPIO_Config();
	SPI_Config(SPI1);
// 	NRF_GetStatus();
  i = NRF24L01_Check();
	NRF_Exti_Config();	
// 	NRF24L01_TX_Mode();
	NRF24L01_RX_Mode();
}
