#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "definitions.h"


extern volatile u32 sys_tick_ms;

void	systick_init(void);
void 	Delay_ms(u16 nms);

//use loop to simulate us delay, not accuracy
void 	Delay_us(u32 nus);

#endif /* SYSTICK_H_ */
