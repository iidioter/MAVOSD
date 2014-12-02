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

#include "usart.h"
#include "osdvar.h"
#include "OSDMavlink.h"

MAVLINK_BUFDef mavlink_buf[10];
int8_t cur_mav_recv_buf_index = -1;
int8_t last_mav_recv_buf_index = -1;
u8 max_mav_msg_length = 0;

void usart_init(u32 bound)
{
	//init the mavlink recv buf
	for(u8 i=0; i<MAV_BUF_COUNT; i++)
	{
		mavlink_buf[i].buf_state = USART_MAVBUF_RDYRECV;
		mavlink_buf[i].buf_length = 0;
	}
	
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
 	
	//reset USART2
	USART_DeInit(USART2);
	
	//Usart2 NVIC config
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
	
	//USART init
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_ERR, ENABLE);
    USART_Cmd(USART2, ENABLE);
}

void usart_send_byte(u8 ch)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}
	USART_SendData(USART2, (u8) ch);
}

void usart_send_str(u8 *buf, u32 len)
{
	while(len != 0)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}
		USART_SendData(USART2, (u8) *buf);
		buf++;
		len--;
	}
}

u8 usart_available(void)
{
	//return USART1->ISR & (1<<5);
	return USART_GetFlagStatus(USART2, USART_FLAG_RXNE);
}

u8 usart_get_char(void)
{
	//return (USART1->RDR);
	return USART_ReceiveData(USART2);
}

int8_t usart_available_packet_to_proc(void)
{
	u8 i = 0;
	for(i=0; i<10; i++)
	{
		if(mavlink_buf[i].buf_state == USART_MAVBUF_RDYPROC)
			return i;
	}

	return -1;
}

int8_t usart_available_packet_to_recv(void)
{
	u8 i = 0;
	for(i=0; i<10; i++)
	{
		if(mavlink_buf[i].buf_state == USART_MAVBUF_RDYRECV)
			return i;
	}

	return -1;
}

//USART2 interrupt process
void USART2_IRQHandler(void) 
{
	u8 c;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		//read recv data
		c =USART_ReceiveData(USART2);
		
//		//process font uploading
//		if (font_uploading == 0) 
//		{
//			if (c == '*')
//			{
//				font_uploading = 1;
//				return;
//			}
//			
//		}
//		else if(font_uploading == 1)
//		{
//			if (c == '*') 
//			{
//				font_uploading = 0;
//				return;
//			}
//		}

//		if(font_uploading == 1)
//		{
//			uploadFont(c);
//		}

		
		//Process mavlink msg
		if(c == 0xFE)	//Denotes the start of mavlink message frame transmission
		{
			if(last_mav_recv_buf_index != -1)
			{
				mavlink_buf[last_mav_recv_buf_index].buf_state = USART_MAVBUF_RDYPROC;
				if(mavlink_buf[last_mav_recv_buf_index].buf_length > max_mav_msg_length)
					max_mav_msg_length = mavlink_buf[last_mav_recv_buf_index].buf_length;
			}

			cur_mav_recv_buf_index = usart_available_packet_to_recv();
			if(cur_mav_recv_buf_index != -1)
			{
				mavlink_buf[cur_mav_recv_buf_index].buf_state = USART_MAVBUF_RECVING;
				mavlink_buf[cur_mav_recv_buf_index].buf_length = 0;
				last_mav_recv_buf_index = cur_mav_recv_buf_index;
			}
			else
			{
				u8 i = 1;
			}
		}
		if(cur_mav_recv_buf_index != -1)
		{
			mavlink_buf[cur_mav_recv_buf_index].mavbuf[mavlink_buf[cur_mav_recv_buf_index].buf_length] = c;
			mavlink_buf[cur_mav_recv_buf_index].buf_length++;
		}
		
	} 
	
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);
		USART_ReceiveData(USART1);
	}
}
