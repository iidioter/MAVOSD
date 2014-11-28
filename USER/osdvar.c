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

#include "osdvar.h"

/////////////////////////////////////////////////////////////////////////
u8		mavbeat = 0;
float 	lastMAVBeat = 0;
u32		lastWritePanel = 0;
u8		waitingMAVBeats = 1;
u8		apm_mav_type;
u8		apm_mav_system; 
u8		apm_mav_component;
u8		enable_mav_request = 0;

/////////////////////////////////////////////////////////////////////////
float        osd_vbat_A = 0;                 // Battery A voltage in milivolt
int16_t      osd_curr_A = 0;                 // Battery A current
int8_t       osd_battery_remaining_A = 0;    // 0 to 100 <=> 0 to 1000

int8_t       osd_pitch = 0;                  // pitch from DCM
int8_t       osd_roll = 0;                   // roll from DCM
int8_t       osd_yaw = 0;                    // relative heading form DCM
float        osd_heading = 0;                // ground course heading from GPS

float        osd_lat = 0;                    // latidude
float        osd_lon = 0;                    // longitude
uint8_t      osd_satellites_visible = 0;     // number of satelites
uint8_t      osd_fix_type = 0;               // GPS lock 0-1=no fix, 2=2D, 3=3D

float        osd_airspeed = -1;              // airspeed
float        osd_groundspeed = 0;            // ground speed
uint16_t     osd_throttle = 0;               // throtle
float        osd_alt = 0;                    // altitude
float        osd_climb = 0;

float	    nav_roll = 0; // Current desired roll in degrees
float        nav_pitch = 0; // Current desired pitch in degrees
int16_t	    nav_bearing = 0; // Current desired heading in degrees
int16_t	    wp_target_bearing = 0; // Bearing to current MISSION/target in degrees
int8_t       wp_target_bearing_rotate_int = 0;
uint16_t     wp_dist = 0; // Distance to active MISSION in meters
uint8_t      wp_number = 0; // Current waypoint number
float	    alt_error = 0; // Current altitude error in meters
float        aspd_error = 0; // Current airspeed error in meters/second
float	    xtrack_error = 0; // Current crosstrack error on x-y plane in meters
float        eff = 0; //Efficiency

int16_t      chan1_raw = 0;
int16_t      chan2_raw = 0;
uint16_t     osd_chan5_raw = 1000;
uint16_t     osd_chan6_raw = 1000;
uint16_t     osd_chan7_raw = 1000;
uint16_t     osd_chan8_raw = 1000;
uint8_t      osd_rssi = 0; //raw value from mavlink

/////////////////////////////////////////////////////////////////////////
u8			request_next_font=0;
u8			cur_recv_buf_index=0;
u16			font_count = 0;
u8			character_bitmap[0x40];
u8			character_bitmap2[0x40];
u8			font_uploading = 0;

/////////////////////////////////////////////////////////////////////////
uint8_t		_panSpeed_XY[2] = {1, 8};
uint8_t		_panThrottle_XY[2] = {1, 2};
uint8_t		_panVehicleAlt_XY[2] = {22, 8};
uint8_t		_panPitch_XY[2] = {8, 2};
uint8_t		_panRoll_XY[2] = {14, 2};
uint8_t		_panHomeDir_XY[2] = {22, 2};
uint8_t		_panHomeDist_XY[2] = {23, 3};
uint8_t		_panMode_XY[2] = {22, 5};
uint8_t		_panTime_XY[2] = {23, 6};
uint8_t		_panHorizon_XY[2] = {8, 6};
uint8_t		_panGPSSats_XY[2] = {1, 10};
uint8_t		_panGPSCoord_XY[2] = {1, 11};
uint8_t		_panBatteryVol_XY[2] = {22, 9};
uint8_t		_panBatteryCurrent_XY[2] = {22, 10};
uint8_t		_panBatteryPercent_XY[2] = {19, 11};
uint8_t		_panBatteryConsume_XY[2] = {22, 12};
uint8_t		_panWPDir_XY[2] = {1, 4};
uint8_t		_panWPDist_XY[2] = {2, 5};



