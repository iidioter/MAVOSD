#ifndef __SPI_H
#define __SPI_H

#include "definitions.h"

//初始化SPI口
void	SPI1_Init(void);			 

//设置SPI速度   
void	SPI1_SetSpeed(u8 SpeedSet);

//SPI总线读写一个字节
u8		SPI1_TransferByte(u8 TxData);
	
#endif

