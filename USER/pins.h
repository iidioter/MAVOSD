#ifndef PINS_H_
#define PINS_H_

#define LED1_PIN        	GPIO_Pin_1
#define LED1_PORT       	GPIOA

#define MAX7456_SPI_PIN		GPIO_Pin_0
#define MAX7456_SPI_PORT	GPIOB

void GPIO_Config(void);

#endif /* PINS_H_ */
