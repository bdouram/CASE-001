/*
 * pins.h
 *
 *  Created on: May 23, 2020
 *      Author: bdouram
 */

#ifndef PINS_H_
#define PINS_H_

/**** Port A ****/
#define PORT_TEST						GPIOA
#define PIN_TEST						GPIO_PIN_0

#define PORT_INHERITANCE				GPIOA
#define PIN_INHERITANCE					GPIO_PIN_1

#define PORT_SCHEDULER					GPIOA
#define PIN_SCHEDULER					GPIO_PIN_4

/**** Port B ****/
#define PORT_END_TASK					GPIOB
#define PIN_END_TASK					GPIO_PIN_0

/**** Port C ****/
#define PORT_TASK_ONE					GPIOC
#define PIN_TASK_ONE					GPIO_PIN_0

#define PORT_TASK_TWO					GPIOC
#define PIN_TASK_TWO					GPIO_PIN_1

#define PORT_TASK_THREE					GPIOC
#define PIN_TASK_THREE					GPIO_PIN_2

#define PORT_TICK						GPIOC
#define PIN_TICK						GPIO_PIN_3

#endif /* PINS_H_ */
