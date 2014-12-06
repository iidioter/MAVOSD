#ifndef OSD_FONT_H_
#define OSD_FONT_H_

#include "definitions.h"

enum SCALE_LINE
{
	SCALE_LINE_LONG_WHITE,
	SCALE_LINE_LONG_BLACK,
	SCALE_LINE_SHORT_WHITE,
	SCALE_LINE_SHORT_BLACK,
	SCALE_LINE_NORMAL,
};

void initScale(void);
void generateUploadScaleFont(void);
void genWhiteLine(u16 row, u8 mode);
void genBlackLine(u16 row, u8 mode);
void genNormal(u16 row);
void moveScaleArr(u8 step);


#endif /* OSD_FONT_H_ */
