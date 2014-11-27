#include <math.h>
#include "utils.h"
#include "pins.h"
#include "definitions.h"

void LEDtoggle(void)
{
    __disable_irq();
    GPIO_ToggleBits(LED1_PORT, LED1_PIN);
    __enable_irq();
}

void Delay_us(unsigned int us)
{
    us *= STM32_DELAY_US_MULT;

		while(us--);
}

void Delay_ms(unsigned int ms)
{
    Delay_us(1000 * ms);
}

float Rad2Deg(float x)
{
    return x * (180.0F / M_PI);
}

float Deg2Rad(float x)
{
    return x * (M_PI / 180.0F);
}

float Round(float x)
{
    if (x >= 0)
    {
        return x + 0.5F;
    }
    else
    {
        return x - 0.5F;
    }
}
