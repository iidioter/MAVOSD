#ifndef OSDMAVLINK_H
#define OSDMAVLINK_H

#pragma anon_unions

#include "definitions.h"
#include "usart.h"
// we have separate helpers disabled to make it possible
// to select MAVLink 1.0 in the arduino GUI build
//#define MAVLINK_SEPARATE_HELPERS

#include "include/mavlink/v1.0/ardupilotmega/version.h"

// this allows us to make mavlink_message_t much smaller
#define MAVLINK_MAX_PAYLOAD_LEN MAVLINK_MAX_DIALECT_PAYLOAD_SIZE

#define MAVLINK_COMM_NUM_BUFFERS 1
#include "include/mavlink/v1.0/mavlink_types.h"


/// MAVLink system definition
extern mavlink_system_t mavlink_system;

/// Send a byte to the nominated MAVLink channel
///
/// @param chan		Channel to send to
/// @param ch		Byte to send
///
static inline void comm_send_ch(mavlink_channel_t chan, uint8_t ch)
{
    switch(chan) {
	case MAVLINK_COMM_0:
		usart_send_byte(ch);
		break;
	default:
		break;
	}
}
#define MAVLINK_USE_CONVENIENCE_FUNCTIONS
#include "include/mavlink/v1.0/ardupilotmega/mavlink.h"

void request_mavlink_rates(void);

void uploadFont(u8 c);

void process_mavlink2(void);

// true when we have received at least 1 MAVLink packet
//extern u8 mavlink_active;
//extern u8 crlf_count;
//
//extern u32 packet_drops;
//extern u32 parse_error;





#endif

