#include <string.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "trace_macros.h"
#include "pins.h"

void xTraceTaskReady(char *task) {
	if (strcmp(task, "Task-1") == 0) {
		HAL_GPIO_WritePin(PORT_TASK_ONE, PIN_TASK_ONE, GPIO_PIN_SET);
		HAL_GPIO_WritePin(PORT_TASK_ONE, PIN_TASK_ONE, GPIO_PIN_RESET);
	} else if (strcmp(task, "Task-2") == 0) {
		HAL_GPIO_WritePin(PORT_TASK_TWO, PIN_TASK_TWO, GPIO_PIN_SET);
		HAL_GPIO_WritePin(PORT_TASK_TWO, PIN_TASK_TWO, GPIO_PIN_RESET);
	} else if (strcmp(task, "Task-3") == 0) {
		HAL_GPIO_WritePin(PORT_TASK_THREE, PIN_TASK_THREE, GPIO_PIN_SET);
		HAL_GPIO_WritePin(PORT_TASK_THREE, PIN_TASK_THREE, GPIO_PIN_RESET);
	} else if (strcmp(task, "Task-4") == 0) {
		HAL_GPIO_WritePin(PORT_INHERITANCE, PIN_INHERITANCE, GPIO_PIN_SET);
		HAL_GPIO_WritePin(PORT_INHERITANCE, PIN_INHERITANCE, GPIO_PIN_RESET);
	}
}

void xTraceTaskIn(char *task) {
	if (strcmp(task, "Task-1") == 0) {
		HAL_GPIO_WritePin(PORT_TASK_ONE, PIN_TASK_ONE, GPIO_PIN_SET);
	} else if (strcmp(task, "Task-2") == 0) {
		HAL_GPIO_WritePin(PORT_TASK_TWO, PIN_TASK_TWO, GPIO_PIN_SET);
	} else if (strcmp(task, "Task-3") == 0) {
		HAL_GPIO_WritePin(PORT_TASK_THREE, PIN_TASK_THREE, GPIO_PIN_SET);
	} else if (strcmp(task, "Task-4") == 0) {
		HAL_GPIO_WritePin(PORT_INHERITANCE, PIN_INHERITANCE, GPIO_PIN_SET);
	}
}

void xTraceTaskOut(char *task) {
	if (strcmp(task, "Task-1") == 0) {
		HAL_GPIO_WritePin(PORT_TASK_ONE, PIN_TASK_ONE, GPIO_PIN_RESET);
	} else if (strcmp(task, "Task-2") == 0) {
		HAL_GPIO_WritePin(PORT_TASK_TWO, PIN_TASK_TWO, GPIO_PIN_RESET);
	} else if (strcmp(task, "Task-3") == 0) {
		HAL_GPIO_WritePin(PORT_TASK_THREE, PIN_TASK_THREE, GPIO_PIN_RESET);
	} else if (strcmp(task, "Task-4") == 0) {
		HAL_GPIO_WritePin(PORT_INHERITANCE, PIN_INHERITANCE, GPIO_PIN_RESET);
	}
}

void xTraceTaskInherit() {
	HAL_GPIO_WritePin(PORT_INHERITANCE, PIN_INHERITANCE, GPIO_PIN_SET);
}

void xTraceTaskDisinherit() {
	HAL_GPIO_WritePin(PORT_INHERITANCE, PIN_INHERITANCE, GPIO_PIN_RESET);
}

void xTraceTaskEnd() {
	HAL_GPIO_WritePin(PORT_END_TASK, PIN_END_TASK, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PORT_END_TASK, PIN_END_TASK, GPIO_PIN_RESET);
}

