#include <stdio.h>
#include "definitions.h"
#include "systick.h"
#include "utils.h"
#include "pins.h"
#include "max7456.h"
#include "usart.h"

uint8_t io_redirect = IO_REDIRECT_USART;
//redirect IO output
int fputc(int ch, FILE *f)
{
	if(io_redirect == IO_REDIRECT_MAX7456)
	{
		SPI_MAX7456_write(ch);
	}
	else if(io_redirect == IO_REDIRECT_USART)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}
		USART_SendData(USART2, (u8) ch);
	}
   
    return 0;
}

void Periph_clock_enable(void)
{
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
							RCC_APB2Periph_SPI1, ENABLE);
	
}

void setup(void)
{
    InitSysTick();

    Periph_clock_enable();
    GPIO_Config();
	
	SPI_MAX7456_init();
	usart_init(57600);
}

int main(void)
{
	setup();
	
	SPI_MAX7456_setPanel(10, 3);
	SPI_MAX7456_openPanel();
	printf("%c%c%c|%c%c%c", 0x30, 0x31, 0x32, 0x33, 0x34, 0x35);
	SPI_MAX7456_closePanel();
	
	while(1)
	{
		LED_ON;
		//printf("LEDON\r\n");
		Delay_us(1000000);
		//printf("LEDOFF\r\n");
		LED_OFF;
		Delay_us(1000000);
	}
}

