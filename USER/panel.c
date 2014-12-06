#include "panel.h"
#include "max7456.h"
#include "osdvar.h"
#include "math.h"
#include "systick.h"
#include <stdio.h>

u8			_lastHorizonColHit[12] = {9,9,9,9,9,9,9,9,9,9,9,9};
u8			_lastHorizonRowHit[12] = {6,6,6,6,6,6,6,6,6,6,6,6};
u8			_HorizonHitIndex = 0;

void writePanels(void)
{
	if(sys_tick_ms < (lastMAVBeat + 2200))
	{
		SPI_MAX7456_setPanel(_panHorizon_XY[0], _panHorizon_XY[1]);
		SPI_MAX7456_openPanel();
		printf("\xDA\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xDB|");
		printf("\xDA\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xDB|");
		printf("\xD8\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xD9|");
		printf("\xDA\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xDB|");
		printf("\xDA\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xDB");
		SPI_MAX7456_closePanel();

		SPI_MAX7456_setPanel(_panPitch_XY[0], _panPitch_XY[1]);
		SPI_MAX7456_openPanel();
		printf("%4i%c%c",osd_pitch,0xb0,0xb1);
		SPI_MAX7456_closePanel();

		SPI_MAX7456_setPanel(_panRoll_XY[0], _panRoll_XY[1]);
		SPI_MAX7456_openPanel();
		printf("%4i%c%c",osd_roll, 0xb0, 0xb2);
		SPI_MAX7456_closePanel();

		showHorizon(_panHorizon_XY[0] + 1, _panHorizon_XY[1]);
	} 
	else 
	{ 
		// if no mavlink update for 2 secs
		// this could be replaced with a No Mavlink warning so the last seen values still show

		SPI_MAX7456_clear();
		waitingMAVBeats = 1;

		// Display our logo and wait... 
		panWaitMAVBeats(5,10); //Waiting for MAVBeats...
	}
}

void panWaitMAVBeats(u8 first_col, u8 first_line)
{
	SPI_MAX7456_setPanel(first_col, first_line);
	SPI_MAX7456_openPanel();
	printf("Waiting for|MAVLink heartbeats...");
	SPI_MAX7456_closePanel();
}

// Calculate and shows Artificial Horizon
void showHorizon(uint8_t start_col, uint8_t start_row) 
{ 
	int32_t x, nose, row, minval, hit, subval = 0;
	const int32_t cols = 12;
	const int32_t rows = 5;
	int32_t col_hit[cols];
	float  pitch, roll;

	if(fabs(osd_pitch) == 90)
		pitch = 89.99 * (90/osd_pitch) * -0.017453293;
	else
		pitch = osd_pitch * -0.017453293;

	if(fabs(osd_roll) == 90)
		roll = 89.99 * (90/osd_roll) * 0.017453293;
	else
		roll = osd_roll * 0.017453293;
	
	nose = round(tan(pitch) * (rows*9));
	for(int32_t col=1;col <= cols;col++)
	{
		//center X point at middle of each col
		x = (col * 12) - (cols * 6) - 6;
		
		//calculating hit point on Y plus offset to eliminate negative values
		col_hit[col-1] = (tan(roll) * x) + nose + (rows*9) - 1;
	}

	//clear the last display
	for(_HorizonHitIndex=0;_HorizonHitIndex < cols; _HorizonHitIndex++)
	{
		SPI_MAX7456_writeSingle(_lastHorizonColHit[_HorizonHitIndex], _lastHorizonRowHit[_HorizonHitIndex], 0x20);
	}
	
	_HorizonHitIndex = 0;
	for(int32_t col=0;col < cols; col++){
		hit = col_hit[col];
		if(hit > 0 && hit < (rows * 18)){
			row = rows - ((hit-1)/18);
			minval = rows*18 - row*18 + 1;
			subval = hit - minval;
			subval = round((subval*9)/18);
			if(subval == 0) subval = 1;
			printHit(start_col + col, start_row + row - 1, subval);
		}
	}
}

void printHit(uint8_t col, uint8_t row, uint8_t subval)
{
	_lastHorizonColHit[_HorizonHitIndex] = col;
	_lastHorizonRowHit[_HorizonHitIndex] = row;
	_HorizonHitIndex++;

	u8 subval_char = 0x05 + subval;
	SPI_MAX7456_writeSingle(col, row, subval_char);
}

