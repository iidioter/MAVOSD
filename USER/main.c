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
#include "osdvar.h"
#include "OSDMavlink.h"
#include "systick.h"

uint8_t io_redirect = IO_REDIRECT_USART;
u32 app_start_time;
//u8 should_process_font_upload;

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
	app_start_time = millis();
	
//	SPI_MAX7456_setPanel(10, 3);
//	SPI_MAX7456_openPanel();
//	printf("%c%c|%c%c", 0x90, 0x91, 0x92, 0x93);
//	SPI_MAX7456_closePanel();
	
	while(1)
	{
		//we have received one font, write to max7456 NVM and send response to sender
		if(request_next_font == 1)
		{
			if(cur_recv_buf_index == 0)
				SPI_MAX7456_write_NVM(font_count, character_bitmap2);
			else
				SPI_MAX7456_write_NVM(font_count, character_bitmap);
			font_count++;
			printf("Char Done\n");
			request_next_font = 0;
		}
		
		//if we are uploading font, we do not need to do other thing
		if(font_uploading == 1)
			continue;
		
		if(enable_mav_request == 1)
		{
			//Request6 rate control
			SPI_MAX7456_clear();
			SPI_MAX7456_setPanel(10, 3);
			SPI_MAX7456_openPanel();
			printf("Requesting DataStreams...");
			SPI_MAX7456_closePanel();

			for(int n = 0; n < 3; n++){
				request_mavlink_rates();//Three times to certify it will be readed
				Delay_ms(50);
			}
			enable_mav_request = 0;
			Delay_ms(50);
			SPI_MAX7456_clear();
			waitingMAVBeats = 0;
			lastMAVBeat = micros();
			lastWritePanel = micros();
		}

		//read_mavlink();
		process_mavlink2();

		if((lastWritePanel+50) > micros())
		{
			//writePanels();
			lastWritePanel = micros();
		}
		
//		LED_ON;
//		//printf("LEDON\r\n");
//		Delay_us(1000000);
//		//printf("LEDOFF\r\n");
//		LED_OFF;
//		Delay_us(1000000);
	}
}

