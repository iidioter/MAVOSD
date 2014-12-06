#ifndef __OSDPANEL_H
#define __OSDPANEL_H 	

#include "definitions.h"


void writePanels(void);

void panWaitMAVBeats(u8 first_col, u8 first_line);
void showHorizon(uint8_t start_col, uint8_t start_row);
void printHit(uint8_t col, uint8_t row, uint8_t subval);

#endif //__OSDPANEL_H
