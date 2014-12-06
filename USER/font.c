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

#include "font.h"
#include "max7456.h"
#include "systick.h"

#define FONT_BUF_SIZE  	1748
#define SCALE_LINES		72

static u8 fontbuf[FONT_BUF_SIZE];
static u8 speedscale[SCALE_LINES];
static u16 scaleStartPos = 0x90;

void initScale()
{
	u32 i = 0;
	u32 last_long_white = 0;
	
	for(i=0; i<SCALE_LINES;i++)
	{
		speedscale[i] = SCALE_LINE_NORMAL;
		if((i%5 == 0) || ((i%5 == 2))) //short black
		{
			speedscale[i] = SCALE_LINE_SHORT_BLACK;
		}
		else if(i%5 == 1) //short white or long white
		{
			if((i==11) || (i==36) || (i==61))
				speedscale[i] = SCALE_LINE_LONG_WHITE;
			else
				speedscale[i] = SCALE_LINE_SHORT_WHITE;
		}
	}
	generateUploadScaleFont();
	
	for(i=0; i<12; i++)
	{
		moveScaleArr(1);
		generateUploadScaleFont();
	}
	
}

void moveScaleArr(u8 step)
{
	u8 i, j, tmp;
	
	step = step * 2;
	
	//right loop move the array 
	for(i=0; i<step; i++)
	{  
		tmp = speedscale[SCALE_LINES-1];
		for(j=SCALE_LINES-1; j>0; j--)
			speedscale[j] = speedscale[j-1];
		speedscale[0] = tmp;
	}
}

void generateUploadScaleFont(void)
{
	u32 i,j;
	u8	character_bitmap[0x40];
	u32 byte_count = 0;

	for(i=0; i<SCALE_LINES; i++)
	{
		if(speedscale[i] == SCALE_LINE_LONG_WHITE)
		{
			genWhiteLine(i,0);
		}
		else if(speedscale[i] == SCALE_LINE_LONG_BLACK)
		{
			genBlackLine(i,0);
		}
		else if(speedscale[i] == SCALE_LINE_SHORT_WHITE)
		{
			genWhiteLine(i,1);
		}
		else if(speedscale[i] == SCALE_LINE_SHORT_BLACK)
		{
			genBlackLine(i,1);
		}
		else
		{
			genNormal(i);
		}
	}

	for(j=0; j<FONT_BUF_SIZE;)
	{
		u8 ascii_byte = 0;
		if (fontbuf[j++] == 1) ascii_byte = ascii_byte + 128;
		if (fontbuf[j++] == 1) ascii_byte = ascii_byte + 64;
		if (fontbuf[j++] == 1) ascii_byte = ascii_byte + 32;
		if (fontbuf[j++] == 1) ascii_byte = ascii_byte + 16;
		if (fontbuf[j++] == 1) ascii_byte = ascii_byte + 8;
		if (fontbuf[j++] == 1) ascii_byte = ascii_byte + 4;
		if (fontbuf[j++] == 1) ascii_byte = ascii_byte + 2;
		if (fontbuf[j++] == 1) ascii_byte = ascii_byte + 1;

		character_bitmap[byte_count] = ascii_byte;
		
		if((j != 0) && (j%432 == 0))
		{
			for(; byte_count<64; byte_count++)
			{
				character_bitmap[byte_count] = 85;
			}
			//one font,we add adition byte and upload
			SPI_MAX7456_write_NVM(scaleStartPos, character_bitmap);
			SPI_MAX7456_reset();
			Delay_ms(10);
			scaleStartPos++;
			byte_count = 0;
		}
		else
		{
			byte_count++;
		}
	}
	//SPI_MAX7456_reset();
}

void genWhiteLine(u16 row, u8 mode)
{
	u32 i=0;
	u32 index=row*24;
	u32 nlimit;
	i=index;
	if(mode==0) 
		nlimit=index+16;
	else
		nlimit=index+10;
	for(; i<=nlimit;)
	{
		fontbuf[i++] = 1;
		fontbuf[i++] = 0;
	}
	fontbuf[i++] = 0;
	fontbuf[i++] = 0;
	
	nlimit=index+24;
	for(; i<nlimit;)
	{
		fontbuf[i++] = 0;
		fontbuf[i++] = 1;
	}
}

void genBlackLine(u16 row, u8 mode)
{
	u32 i=0;
	u32 index=row*24;
	i=index;
	u32 nlimit;
	if(mode==0) 
		nlimit=index+16;
	else
		nlimit=index+10;
	fontbuf[i++] = 1;
	fontbuf[i++] = 0;
	for(; i<=nlimit;)
	{
		fontbuf[i++] = 0;
		fontbuf[i++] = 0;
	}
	
	nlimit=index+24;
	for(; i<nlimit;)
	{
		fontbuf[i++] = 0;
		fontbuf[i++] = 1;
	}
}

void genNormal(u16 row)
{
	u32 i=0;
	u32 index=row*24;
	i=index;
	u16 nlimit;
	fontbuf[i++] = 1;
	fontbuf[i++] = 0;
	fontbuf[i++] = 0;
	fontbuf[i++] = 0;
	
	nlimit=index+24;
	for(; i<nlimit;)
	{
		fontbuf[i++] = 0;
		fontbuf[i++] = 1;
	}
}
