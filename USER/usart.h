#ifndef USART_H_
#define USART_H_

#include "definitions.h"

#define USART_FONT_RX_BUF_LEN  	1000
extern u8  USART_FONT_RX_BUF[USART_FONT_RX_BUF_LEN];

enum MAVBUF_STATE
{
	USART_MAVBUF_RDYRECV,
	USART_MAVBUF_RDYPROC,
	USART_MAVBUF_RECVING,
	USART_MAVBUF_PROCING,
};

typedef struct  
{
	u8		buf_state;
	u8		buf_length;
	u8		mavbuf[128];

}MAVLINK_BUFDef;

extern MAVLINK_BUFDef mavlink_buf[10]; 
extern int8_t cur_mav_recv_buf_index;
extern int8_t last_mav_recv_buf_index;
extern u8 max_mav_msg_length;


void 	usart_init(u32 bound);
void 	usart_send_byte(u8 ch);
void 	usart_send_str(u8 *buf, u32 len);
u8		usart_available(void);
u8		usart_get_char(void);

int8_t	usart_available_packet_to_proc(void);
int8_t	usart_available_packet_to_recv(void);

#endif /* USART_H_ */