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
#include "panel.h"
#include "font.h"

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
    systick_init();

    Periph_clock_enable();
    GPIO_Config();
	
	SPI_MAX7456_init();
	usart_init(57600);
	
}

void printALL(void)
{
	u8 i,j,k;
	for(i=0; i<15; i++)
	{
		for(j=4; j<20; j++)
		{
			SPI_MAX7456_writeSingle(j, i, i*16+j-4);
		}
	}
}

int main(void)
{
	setup();
	app_start_time = sys_tick_ms;
	
	//initScale();
	//printALL();
	//read_one_char_from_NVM(1);
	
//	SPI_MAX7456_setPanel(10, 3);
//	SPI_MAX7456_openPanel();
//	printf("%c|%c|%c|%c|%c|%c", 0x9f, 0xaf, 0xbf, 0xcf, 0xDf, 0xEf);
//	//printf("%c", 0xE0);
//	SPI_MAX7456_closePanel();
	u32 last_print_scale = sys_tick_ms;
	u8 j=0x90;
//	for(i=4; i<17; i++)
//	{
//		SPI_MAX7456_setPanel(i, 3);
//		SPI_MAX7456_openPanel();
//		printf("%c|%c|%c|%c", j, j+1, j+2, j+3);
//		SPI_MAX7456_closePanel();
//		j = j+4;
//	}
	SPI_MAX7456_clear();
	while(1)
	{
		if((last_print_scale+50000) > sys_tick_ms)
		{
			SPI_MAX7456_setPanel(10, 3);
			SPI_MAX7456_openPanel();
			printf("%c|%c|%c|%c", j, j+1, j+2, j+3);
			SPI_MAX7456_closePanel();
			j = j+4;
			if(j > 192)
				j = 0x90;
			last_print_scale = sys_tick_ms;
		}
		
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
//		
//		//if we are uploading font, we do not need to do other thing
//		if(font_uploading == 1)
//		{
//			continue;
//		}
		
//		if(enable_mav_request == 1)
//		{
//			//Request6 rate control
//			SPI_MAX7456_clear();
//			SPI_MAX7456_setPanel(10, 3);
//			SPI_MAX7456_openPanel();
//			printf("Requesting DataStreams...");
//			SPI_MAX7456_closePanel();

//			for(int n = 0; n < 3; n++){
//				request_mavlink_rates();//Three times to certify it will be readed
//				Delay_ms(50);
//			}
//			enable_mav_request = 0;
//			Delay_ms(50);
//			SPI_MAX7456_clear();
//			waitingMAVBeats = 0;
//			lastMAVBeat = sys_tick_ms;
//			lastWritePanel = sys_tick_ms;
//		}

//		//read_mavlink();
//		process_mavlink2();

//		if((lastWritePanel+100) > sys_tick_ms)
//		{
//			writePanels();
//			lastWritePanel = sys_tick_ms;
//		}
//		
//		LED_ON;
//		//printf("LEDON\r\n");
//		Delay_us(1000000);
//		//printf("LEDOFF\r\n");
//		LED_OFF;
//		Delay_ms(1000);
	}
}

