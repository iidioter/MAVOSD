#ifndef UTILS_H_
#define UTILS_H_

//LED 低电平有效
#define LED_ON 		GPIO_ResetBits(LED1_PORT, LED1_PIN)
#define LED_OFF		GPIO_SetBits(LED1_PORT, LED1_PIN)
void LEDtoggle(void);

void Blink(void);

//void Delay_ms(unsigned int ms);
//void Delay_us(unsigned int us);

float Rad2Deg(float x);
float Deg2Rad(float x);
float Round(float x);





#endif /* UTILS_H_ */
