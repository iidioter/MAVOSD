#include "systick.h"


volatile u32 sys_tick_ms = 0;

void systick_init()
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick->CTRL |= (1<<1);  	//中断使能
	SysTick->VAL=0X00;            //当前数值寄存器清零，并清除溢出标志位
	SysTick->LOAD=0X2328;     //计数器赋初值
	SysTick->CTRL |= (1<<0);     //开启计数器
}

void Delay_ms(u16 nms)
{
	u32 now = sys_tick_ms;
	while( sys_tick_ms < (now+nms));
}

void SysTick_Handler(void)
{
	sys_tick_ms++;
}

void Delay_us(u32 nus)
{		
	u16 i=0;
	while(nus--)
	{
		i=6;
		while(i--);
	}
}