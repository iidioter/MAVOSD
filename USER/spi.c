#include "spi.h"

void SPI1_Init(void)
{
  	SPI_InitTypeDef  SPI_InitStructure;
	
	SPI_Cmd(SPI1, DISABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI1, &SPI_InitStructure);  
	
	SPI_Cmd(SPI1, ENABLE);
} 

/**
  *SPI 速度设置函数
  *SPI_BaudRatePrescaler_2   2分频   
  *SPI_BaudRatePrescaler_8   8分频   
  *SPI_BaudRatePrescaler_16  16分频  
  *SPI_BaudRatePrescaler_256 256分频 
  */
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;
	SPI_Cmd(SPI1,ENABLE); 
} 

/**
  *SPIx 读写一个字节
  *TxData:要写入的字节
  *返回值:读取到的字节
  */
u8 SPI1_TransferByte(u8 TxData)
{		
	//检查指定的SPI标志位设置与否:发送缓存空标志位
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	
	 //检查指定的SPI标志位设置与否:接受缓存非空标志位
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}






























