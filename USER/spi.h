#ifndef __SPI_H
#define __SPI_H

#include "definitions.h"

//��ʼ��SPI��
void	SPI1_Init(void);			 

//����SPI�ٶ�   
void	SPI1_SetSpeed(u8 SpeedSet);

//SPI���߶�дһ���ֽ�
u8		SPI1_TransferByte(u8 TxData);
	
#endif

