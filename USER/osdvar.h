#ifndef __OSDVAR_H
#define __OSDVAR_H 	

#include "definitions.h"


/////////////////////////////////////////////////////////////////////////
extern u8		mavbeat;
extern float 	lastMAVBeat;
extern u32		lastWritePanel;
extern u8		waitingMAVBeats;
extern u8		apm_mav_type;
extern u8		apm_mav_system; 
extern u8		apm_mav_component;
extern u8		enable_mav_request;

/////////////////////////////////////////////////////////////////////////
extern float        osd_vbat_A;                 // Battery A voltage in milivolt
extern int16_t      osd_curr_A;                 // Battery A current
extern int8_t       osd_battery_remaining_A;    // 0 to 100 <=> 0 to 1000

extern int8_t       osd_pitch;                  // pitch from DCM
extern int8_t       osd_roll;                   // roll from DCM
extern int8_t       osd_yaw;                    // relative heading form DCM
extern float        osd_heading;                // ground course heading from GPS

extern float        osd_lat;                    // latidude
extern float        osd_lon;                    // longitude
extern uint8_t      osd_satellites_visible;     // number of satelites
extern uint8_t      osd_fix_type;               // GPS lock 0-1=no fix, 2=2D, 3=3D

extern float        osd_airspeed;              // airspeed
extern float        osd_groundspeed;            // ground speed
extern uint16_t     osd_throttle;               // throtle
extern float        osd_alt;                    // altitude
extern float        osd_climb;

extern float	    nav_roll; // Current desired roll in degrees
extern float        nav_pitch; // Current desired pitch in degrees
extern int16_t	    nav_bearing; // Current desired heading in degrees
extern int16_t	    wp_target_bearing; // Bearing to current MISSION/target in degrees
extern int8_t       wp_target_bearing_rotate_int;
extern uint16_t     wp_dist; // Distance to active MISSION in meters
extern uint8_t      wp_number; // Current waypoint number
extern float	    alt_error; // Current altitude error in meters
extern float        aspd_error; // Current airspeed error in meters/second
extern float	    xtrack_error; // Current crosstrack error on x-y plane in meters
extern float        eff; //Efficiency

extern int16_t      chan1_raw;
extern int16_t      chan2_raw;
extern uint16_t     osd_chan5_raw;
extern uint16_t     osd_chan6_raw;
extern uint16_t     osd_chan7_raw;
extern uint16_t     osd_chan8_raw;
extern uint8_t      osd_rssi; //raw value from mavlink

/////////////////////////////////////////////////////////////////////////
extern u8			request_next_font;
extern u8			cur_recv_buf_index;
extern u16			font_count;
extern u8			character_bitmap[0x40];
extern u8			character_bitmap2[0x40];
extern u8			font_uploading;

/////////////////////////////////////////////////////////////////////////
extern uint8_t		_panSpeed_XY[2];
extern uint8_t		_panThrottle_XY[2];
extern uint8_t		_panVehicleAlt_XY[2];
extern uint8_t		_panPitch_XY[2];
extern uint8_t		_panRoll_XY[2];
extern uint8_t		_panHomeDir_XY[2];
extern uint8_t		_panHomeDist_XY[2];
extern uint8_t		_panMode_XY[2];
extern uint8_t		_panTime_XY[2];
extern uint8_t		_panHorizon_XY[2];
extern uint8_t		_panGPSSats_XY[2];
extern uint8_t		_panGPSCoord_XY[2];
extern uint8_t		_panBatteryVol_XY[2];
extern uint8_t		_panBatteryCurrent_XY[2];
extern uint8_t		_panBatteryPercent_XY[2];
extern uint8_t		_panBatteryConsume_XY[2];
extern uint8_t		_panWPDir_XY[2];
extern uint8_t		_panWPDist_XY[2];





#endif
