#ifndef SYSTICK_H_
#define SYSTICK_H_

void InitSysTick(void);
void SysTickAttachCallback(void (*callback)(void));
unsigned int millis(void);
unsigned int micros(void);
#endif /* SYSTICK_H_ */
