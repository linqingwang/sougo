#include "nrf24l01.h"
#include "stm32f10x_spi.h"

// const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
// const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0xFF,0xFF,0xFF,0xFF,0xFF}; //���͵�ַ
extern u8 tmp_buf[1];
u8 Transmit[1] = {START_TRANS};
u8 Trans_Status = 0;

/** NRF24L01 GPIO���� */
static void NRF_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;		// SCK MOSI ���츴�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;					// MISO ��������
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
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;					// IRQ ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	NRF_CE = 0;
	NRF_CSN = 1;
}

/** NRF24L01 �ⲿ�ж����� */
static void NRF_Exti_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
	
	  EXTI_InitStructure.EXTI_Line = EXTI_Line5;				
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 	//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);
}

/** NRF24L01 SPI�������� */
static void SPI_Config(SPI_TypeDef* SPIx)
{
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRCֵ����Ķ���ʽ
	SPI_Init(SPIx, &SPI_InitStructure);  								//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI_Cmd(SPIx, ENABLE);
}

static void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); //�ж���Ч��
	SPI1->CR1 &= 0XFFC7;											//λ3-5���㣬�������ò�����
	SPI1->CR1 |= SPI_BaudRatePrescaler;	    						//����SPI1�ٶ� 
	SPI_Cmd(SPIx,ENABLE); 				   							//ʹ��SPI1
} 

/**
  * @brief  SPI1����дһ���ֽ�
  * @param  TxData��Ҫд����ֽ�
  * @retval ��ȡ�����ֽ�
  */
static u8 NRF_SPI1_ReadWriteByte(u8 TxData)
{		 			 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){} //�ȴ���������  	
	SPI_I2S_SendData(SPI1, TxData); 								 //ͨ������SPIx����һ��byte����		
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}//�ȴ�������һ��byte   
	return SPI_I2S_ReceiveData(SPI1);   							 //����ͨ��SPIx������յ�����			    
}



/**
  * @brief  SPIд�Ĵ���
  * @param  reg��ָ���Ĵ�����ַ
  * @param  value��Ҫд���ֵ
  * @retval �˴ζ�����״̬�Ĵ���ֵ
  */
static u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
	NRF_CSN = 0;    					 //ʹ��SPI����
	status = NRF_SPI1_ReadWriteByte(reg);//���ͼĴ����� 
	NRF_SPI1_ReadWriteByte(value);       //д��Ĵ�����ֵ
	NRF_CSN = 1;      					 //��ֹSPI����	   
	return(status);       				 //����״ֵ̬
}

/**
  * @brief  SPI���Ĵ���
  * @param  reg��ָ���Ĵ�����ַ
  * @retval ��ȡ����ֵ
  */
static u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
	NRF_CSN = 0;       						//ʹ��SPI����		
	NRF_SPI1_ReadWriteByte(reg);   			//���ͼĴ�����
	reg_val = NRF_SPI1_ReadWriteByte(NOP); //��ȡ�Ĵ�������
	NRF_CSN = 1;         					//��ֹSPI����		    
	return(reg_val);           				//���ؼĴ����е�ֵ
}	

/**
  * @brief  SPI��ָ��λ�ö���ָ�����ȵ�����
  * @param  reg��ָ���Ĵ�����ַ
  * @param  *puf���洢��ȡ���ݵ�����ָ��
  * @param  len�����ݳ���
  * @retval �˴ζ�����״̬�Ĵ���ֵ
  */ 
static u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
	NRF_CSN = 0;           	//ʹ��SPI����
	status = NRF_SPI1_ReadWriteByte(reg);			//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
		pBuf[u8_ctr] = NRF_SPI1_ReadWriteByte(0XFF);//��������
	NRF_CSN = 1;       			//�ر�SPI����
	return status;        
}

/**
  * @brief  SPI��ָ��λ��д��ָ�����ȵ�����
  * @param  reg��ָ���Ĵ�����ַ
  * @param  *puf���洢Ҫд�����ݵ�����ָ��
  * @param  len�����ݳ���
  * @retval �˴ζ�����״̬�Ĵ���ֵ
  */
static u8 NRF24L01_Write_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	    
	NRF_CSN = 0;       //ʹ��SPI����
	status = NRF_SPI1_ReadWriteByte(reg);	//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	for(u8_ctr=0; u8_ctr<len; u8_ctr++)
		NRF_SPI1_ReadWriteByte(*pBuf++); 	//д������	 
	NRF_CSN = 1;       	//�ر�SPI����
	return status;          				//���ض�����״ֵ̬
}

/**
  * @brief  ��ȡNRF24L01 STATUS�Ĵ���״̬
  * @param  TxData��Ҫд����ֽ�
  * @retval STATUS�Ĵ�����ֵ
  */
static u8 NRF_GetStatus(void)
{
	u8 status;
	status = NRF24L01_Read_Reg(STATUS);
	return status;
}

/**
  * @brief  ���NRF24L01�Ƿ����
  * @param  ��
  * @retval 1��������
  * @retval 0������
  */
static u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	SPI_SetSpeed(SPI1,SPI_BaudRatePrescaler_8); 	//spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz��   	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); 				//����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;								//���24L01����	
	return 0;		 								//��⵽24L01
}	

/**
  * @brief  ��ʼ��NRF24L01��TXģʽ������ģʽ
  * @param  ��
  * @retval ��
  * @note   ����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
  *         PWR_UP,CRCʹ�ܣ�CEΪ�ߴ���10us,����������
  */	 
static void NRF24L01_TX_Mode(void)
{														 
	NRF_CE = 0;	    
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,0);       //����RFͨ��Ϊ40
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF_CE = 1;					  					  //CEΪ��,10us����������
}

/**
  * @brief  ��ʼ��NRF24L01��RXģʽ ����ģʽ
  * @param  ��
  * @retval ��
  * @note   ����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
  *         ��CE��ߺ�,������RXģʽ,�����Խ���������
  */		   
static void NRF24L01_RX_Mode(void)
{
	NRF_CE = 0;	  
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);  //ѡ��ͨ��0����Ч���ݿ�� 
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	
	NRF24L01_Write_Reg(FLUSH_RX,0xff);									//���RX FIFO�Ĵ���   
  
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    			//ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);			//ʹ��ͨ��0�Ľ��յ�ַ  	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,0);	     			//����RFͨ��Ƶ��		  	    
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0F);			//����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);				//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
	NRF_CE = 1; 												//CEΪ��,�������ģʽ 
}

/**
  * @brief  ����NRF24L01����һ������
  * @param  txbuf:�����������׵�ַ
  * @retval MAX_TX������ʧ�ܣ��ﵽ����ط�����
  * @retval TX_OK���������
  * @retval 0xff������ԭ����ʧ��
  */
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	SPI_SetSpeed(SPI1,SPI_BaudRatePrescaler_8);			  //spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	NRF_CE = 0;
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH); //д���ݵ�TX BUF  
 	NRF_CE = 1;						  					  //��������	   
	while (NRF_IRQ != 0);		 						  //�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);  					  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 		  //���TX_DS��MAX_RT�жϱ�־
	if (sta&MAX_TX)										  //�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);				  //���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if (sta&TX_OK)									      //�������
		return TX_OK;	
	return 0xff;										  //����ԭ����ʧ��
}

/**
  * @brief  ����NRF24L01����һ������
  * @param  rxbuf:�洢�����������׵�ַ
  * @retval 1������ʧ�ܣ�û�н��յ��κ�����
  * @retval 0���������
  */
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI_SetSpeed(SPI1,SPI_BaudRatePrescaler_8); 	//spi�ٶ�Ϊ10.5Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta = NRF24L01_Read_Reg(STATUS);  				//��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 	//���TX_DS��MAX_RT�жϱ�־
	if (sta&RX_OK)									//���յ�����
	{ 
		NRF_CE = 0;
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);					//���RX FIFO�Ĵ��� 
		NRF_CE = 1;
// 		delay_150us(); 
		return 0; 
	}	   
	return 1;												//û�յ��κ�����
}

/** NRF24L01 ��ʼ������ */
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
