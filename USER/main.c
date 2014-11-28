/****************************************************************************
 *
 *   Copyright (c) 2014-2014 PlayUAV Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PlayUAV nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlayUAV  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

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

