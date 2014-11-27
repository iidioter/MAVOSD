#include "pins.h"
#include "stm32f10x.h"
#include "utils.h"

void GPIO_Config(void)  //Configures GPIO
{
    GPIO_InitTypeDef    GPIO_InitStructure;
	
    //LED pin config
    GPIO_InitStructure.GPIO_Pin = LED1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	
	//USART2 pin config
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//SPI MOIS MISO SLK pin config
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//SPI CS Pin
	GPIO_InitStructure.GPIO_Pin = MAX7456_SPI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(MAX7456_SPI_PORT, &GPIO_InitStructure);
	GPIO_SetBits(MAX7456_SPI_PORT,MAX7456_SPI_PIN);
}
