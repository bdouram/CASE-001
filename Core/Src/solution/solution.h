/*
 * solution.h
 *
 *  Created on: May 4, 2020
 *      Author: bdouram
 */

#ifndef SRC_SOLUTION_SOLUTION_H_
#define SRC_SOLUTION_SOLUTION_H_

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"


/* SD includes */
#include "sd.h"


void runSolution(void);

/* Task handlers */
void v1Task0_Handler(void *params);
void v1Task1_Handler(void *params);
void v1Task2_Handler(void *params);
void v1Task3_Handler(void *params);


/* ITM Register (for debug only) */
#define ITM_Port32(n)  (*((volatile unsigned long *)(0xE0000000+4*n)))

/*AUX defines*/
#define TRUE		1
#define FALSE		0

/*Archive commands and name files.*/
#define COMMANDS			{'W','R', 'N'}
#define	ARCHIVES_READ		{"0.txt","1.txt","2.txt", "3.txt","4.txt","5.txt","6.txt","7.txt","8.txt","9.txt"}
#define ARCHIVES_WRITE		{"0w.txt","1w.txt","2w.txt","3w.txt","4w.txt","5wr.txt","6w.txt","7w.txt","8w.txt","9w.txt"}


#endif /* SRC_SOLUTION_SOLUTION_H_ */
