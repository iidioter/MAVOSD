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

#include "OSDMavlink.h"
#include "max7456.h"
#include "osdvar.h"
#include "utils.h"

mavlink_message_t msg; 
mavlink_status_t status;
	
u8 mavlink_active;

u32 packet_drops;
u32 parse_error;

mavlink_system_t mavlink_system = {12,1,0,0}; //modified

void request_mavlink_rates(void)
{
    const u32  maxStreams = 6;
    const u8 MAVStreams[maxStreams] = {MAV_DATA_STREAM_RAW_SENSORS,
        MAV_DATA_STREAM_EXTENDED_STATUS,
        MAV_DATA_STREAM_RC_CHANNELS,
        MAV_DATA_STREAM_POSITION,
        MAV_DATA_STREAM_EXTRA1, 
        MAV_DATA_STREAM_EXTRA2};
    const u16 MAVRates[maxStreams] = {0x01, 0x02, 0x05, 0x02, 0x05, 0x02};
    for (u32 i=0; i < maxStreams; i++) {
        mavlink_msg_request_data_stream_send(MAVLINK_COMM_0,
            apm_mav_system, apm_mav_component,
            MAVStreams[i], MAVRates[i], 1);
    }
}

void process_mavlink2()
{
	int8_t proc_index = usart_available_packet_to_proc();
	while(proc_index != -1)
	{
		u8 buf_len = mavlink_buf[proc_index].buf_length;
		u8 char_index = 0;
		mavlink_buf[proc_index].buf_state = USART_MAVBUF_PROCING;
		while(char_index < buf_len + 1)
		{

			u8 c = mavlink_buf[proc_index].mavbuf[char_index];
			char_index++;

			//trying to grab msg  
			if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
				mavlink_active = 1;
				//handle msg
				switch(msg.msgid) {
				case MAVLINK_MSG_ID_HEARTBEAT:
					{
						mavbeat = 1;
						apm_mav_system    = msg.sysid;
						apm_mav_component = msg.compid;
						apm_mav_type      = mavlink_msg_heartbeat_get_type(&msg);            

//						lastMAVBeat = sys_tick_ms;
						if(waitingMAVBeats == 1){
							enable_mav_request = 1;
						}
					}
					break;
				case MAVLINK_MSG_ID_SYS_STATUS:
					{
						osd_vbat_A = (mavlink_msg_sys_status_get_voltage_battery(&msg) / 1000.0f); //Battery voltage, in millivolts (1 = 1 millivolt)
						osd_curr_A = mavlink_msg_sys_status_get_current_battery(&msg); //Battery current, in 10*milliamperes (1 = 10 milliampere)         
						osd_battery_remaining_A = mavlink_msg_sys_status_get_battery_remaining(&msg); //Remaining battery energy: (0%: 0, 100%: 100)
						//osd_mode = apm_mav_component;//Debug
						//osd_nav_mode = apm_mav_system;//Debug
					}
					break;
				case MAVLINK_MSG_ID_GPS_RAW_INT:
					{
						osd_lat = mavlink_msg_gps_raw_int_get_lat(&msg) / 10000000.0f;
						osd_lon = mavlink_msg_gps_raw_int_get_lon(&msg) / 10000000.0f;
						osd_fix_type = mavlink_msg_gps_raw_int_get_fix_type(&msg);
						osd_satellites_visible = mavlink_msg_gps_raw_int_get_satellites_visible(&msg);
					}
					break;
				case MAVLINK_MSG_ID_VFR_HUD:
					{
						osd_airspeed = mavlink_msg_vfr_hud_get_airspeed(&msg);
						osd_groundspeed = mavlink_msg_vfr_hud_get_groundspeed(&msg);
						osd_heading = mavlink_msg_vfr_hud_get_heading(&msg); // 0..360 deg, 0=north
						osd_throttle = mavlink_msg_vfr_hud_get_throttle(&msg);
						//if(osd_throttle > 100 && osd_throttle < 150) osd_throttle = 100;//Temporary fix for ArduPlane 2.28
						//if(osd_throttle < 0 || osd_throttle > 150) osd_throttle = 0;//Temporary fix for ArduPlane 2.28
						osd_alt = mavlink_msg_vfr_hud_get_alt(&msg);
						osd_climb = mavlink_msg_vfr_hud_get_climb(&msg);
					}
					break;
				case MAVLINK_MSG_ID_ATTITUDE:
					{
						osd_pitch = Rad2Deg(mavlink_msg_attitude_get_pitch(&msg));
						osd_roll = Rad2Deg(mavlink_msg_attitude_get_roll(&msg));
						osd_yaw = Rad2Deg(mavlink_msg_attitude_get_yaw(&msg));
					}
					break;
				case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT:
					{
						nav_roll = mavlink_msg_nav_controller_output_get_nav_roll(&msg);
						nav_pitch = mavlink_msg_nav_controller_output_get_nav_pitch(&msg);
						nav_bearing = mavlink_msg_nav_controller_output_get_nav_bearing(&msg);
						wp_target_bearing = mavlink_msg_nav_controller_output_get_target_bearing(&msg);
						wp_dist = mavlink_msg_nav_controller_output_get_wp_dist(&msg);
						alt_error = mavlink_msg_nav_controller_output_get_alt_error(&msg);
						aspd_error = mavlink_msg_nav_controller_output_get_aspd_error(&msg);
						xtrack_error = mavlink_msg_nav_controller_output_get_xtrack_error(&msg);
					}
					break;
				case MAVLINK_MSG_ID_MISSION_CURRENT:
					{
						wp_number = (uint8_t)mavlink_msg_mission_current_get_seq(&msg);
					}
					break;
				case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
					{
						chan1_raw = mavlink_msg_rc_channels_raw_get_chan1_raw(&msg);
						chan2_raw = mavlink_msg_rc_channels_raw_get_chan2_raw(&msg);
						osd_chan5_raw = mavlink_msg_rc_channels_raw_get_chan5_raw(&msg);
						osd_chan6_raw = mavlink_msg_rc_channels_raw_get_chan6_raw(&msg);
						osd_chan7_raw = mavlink_msg_rc_channels_raw_get_chan7_raw(&msg);
						osd_chan8_raw = mavlink_msg_rc_channels_raw_get_chan8_raw(&msg);
						osd_rssi = mavlink_msg_rc_channels_raw_get_rssi(&msg);
					}
					break;
				default:
					//Do nothing
					break;
				}
			}
		}
		mavlink_buf[proc_index].buf_state = USART_MAVBUF_RDYRECV;
		proc_index = usart_available_packet_to_proc();
	}

}


void uploadFont(u8 incomingByte)
{
//	if(incomingByte == 0x0d)
//	{
//		if (bit_count == 8 && (ascii_binary[0] == 0x30 || ascii_binary[0] == 0x31))
//		{
//			u8 ascii_byte;

//            ascii_byte = 0;

//			if (ascii_binary[0] == 0x31) // ascii '1'
//				ascii_byte = ascii_byte + 128;

//			if (ascii_binary[1] == 0x31)
//				ascii_byte = ascii_byte + 64;

//			if (ascii_binary[2] == 0x31)
//				ascii_byte = ascii_byte + 32;

//			if (ascii_binary[3] == 0x31)
//				ascii_byte = ascii_byte + 16;

//			if (ascii_binary[4] == 0x31)
//				ascii_byte = ascii_byte + 8;

//			if (ascii_binary[5] == 0x31)
//				ascii_byte = ascii_byte + 4;

//			if (ascii_binary[6] == 0x31)
//				ascii_byte = ascii_byte + 2;

//			if (ascii_binary[7] == 0x31)
//				ascii_byte = ascii_byte + 1;
//				
//			if(cur_recv_buf_index == 0)
//				character_bitmap[byte_count] = ascii_byte;
//			else
//				character_bitmap2[byte_count] = ascii_byte;
//			
//			byte_count++;
//			bit_count = 0;
//		}
//		else
//		{
//			bit_count = 0;
//		}
//	}
//	else if (incomingByte == 0x30 || incomingByte == 0x31)
//	{
//		ascii_binary[bit_count] = incomingByte;
//		bit_count++;
//	}
	
//	if(byte_count == 64)
//	{
//		byte_count = 0;
//		
//		request_next_font = 1;
//		
//		//change recv buf
//		if(cur_recv_buf_index == 0)
//			cur_recv_buf_index = 1;
//		else
//			cur_recv_buf_index = 0;
//	}
}
