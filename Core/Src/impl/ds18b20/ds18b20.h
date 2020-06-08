/*
 * ds18b20.h
 *
 *  Created on: Jun 3, 2020
 *      Author: bdouram
 */

#ifndef SRC_DS18B20_DS18B20_H_
#define SRC_DS18B20_DS18B20_H_

/*Microseconds tasks prototype*/
void DWTimer_Delay_us(volatile uint32_t microseconds);

/*PIN mode change*/
void PIN_INPUT();
void PIN_OUTPUT();

/*Read and write PIN*/
void WRITE(uint8_t data);
uint8_t READ();

/*DS18B20 methods*/
uint8_t DS18B20_Init();
float DS18B20_Celsius();

#define TEMP_PIN			GPIO_PIN_10
#define TEMP_PORT			GPIOC

#endif /* SRC_DS18B20_DS18B20_H_ */
