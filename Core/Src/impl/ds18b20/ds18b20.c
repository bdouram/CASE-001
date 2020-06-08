/*
 * ds18b20.c
 *
 *  Created on: Jun 3, 2020
 *      Author: bdouram
 */

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "ds18b20.h"


GPIO_InitTypeDef GPIO_InitStruct;

void PIN_INPUT()
{
	GPIO_InitStruct.Pin = TEMP_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(TEMP_PORT, &GPIO_InitStruct);
}

void PIN_OUTPUT()
{
	GPIO_InitStruct.Pin = TEMP_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TEMP_PORT, &GPIO_InitStruct);
}

uint8_t DS18B20_Init() {

	PIN_OUTPUT();
	HAL_GPIO_WritePin(TEMP_PORT, TEMP_PIN, 0); /* Pull the pin low */
	DWTimer_Delay_us(480); /* Delay according to datasheet */

	PIN_INPUT();
	DWTimer_Delay_us(80); /* Delay according to datasheet */

	/* If the pin is low i.e the presence pulse is there */
	if (!(HAL_GPIO_ReadPin(TEMP_PORT, TEMP_PIN))) {
		DWTimer_Delay_us(400); /* Delay according to datasheet */
		return 0;
	} else {
		DWTimer_Delay_us(400); /* Delay according to datasheet */
		return 1;
	}

}

void WRITE(uint8_t data) {

	PIN_OUTPUT();

	for (int i = 0; i < 8; i++) {

		/* If the bit is high */
		if ((data & (1 << i)) != 0) {

			/* Write 1 */
			PIN_OUTPUT(); /* Set as output */
			HAL_GPIO_WritePin(TEMP_PORT, TEMP_PIN, 0); /* Pull the pin LOW */
			DWTimer_Delay_us(1); /* Wait for  us */

			PIN_INPUT(); /* Set as input */
			DWTimer_Delay_us(60); /* Wait for 60 us */
		} else /* If the bit is low */
		{
			/* Write 0 */
			PIN_OUTPUT();
			HAL_GPIO_WritePin(TEMP_PORT, TEMP_PIN, 0); /* Pull the pin LOW */
			DWTimer_Delay_us(60); /* Wait for 60 us */
			PIN_INPUT(); /* Set as intput */
		}
	}
}

uint8_t READ() {

	uint8_t value = 0;
	PIN_INPUT();

	for (int i = 0; i < 8; i++) {
		PIN_OUTPUT(); /* Set as output */

		HAL_GPIO_WritePin(TEMP_PORT, TEMP_PIN, 0); /* Pull the data pin LOW */
		DWTimer_Delay_us(2); /* Wait for 2 us */

		PIN_INPUT(); /* Set as input */
		if (HAL_GPIO_ReadPin(TEMP_PORT, TEMP_PIN))  // if the pin is HIGH
				{
			value |= 1 << i;  // read = 1
		}
		DWTimer_Delay_us(60);  // wait for 60 us
	}

	return value;
}


float DS18B20_Celsius() {

	uint8_t temp_l = 0x00, temp_h = 0x00;

	DS18B20_Init();
	WRITE(0xCC);  // skip ROM
	WRITE(0x44);  // convert t
	DWTimer_Delay_us(800);

	DS18B20_Init();
	WRITE(0xCC);  // skip ROM
	WRITE(0xBE);  // Read Scratchpad

	temp_l = READ();
	temp_h = READ();
	return (float) ((temp_h << 8) | temp_l) / 16;

}



void DWTimer_Delay_us(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;
  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}
