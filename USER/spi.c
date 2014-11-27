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
  *SPI �ٶ����ú���
  *SPI_BaudRatePrescaler_2   2��Ƶ   
  *SPI_BaudRatePrescaler_8   8��Ƶ   
  *SPI_BaudRatePrescaler_16  16��Ƶ  
  *SPI_BaudRatePrescaler_256 256��Ƶ 
  */
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;
	SPI_Cmd(SPI1,ENABLE); 
} 

/**
  *SPIx ��дһ���ֽ�
  *TxData:Ҫд����ֽ�
  *����ֵ:��ȡ�����ֽ�
  */
u8 SPI1_TransferByte(u8 TxData)
{		
	//���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	
	 //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����					    
}






























