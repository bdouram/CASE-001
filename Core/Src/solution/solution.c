#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "stm32f4xx.h"

#include "fatfs.h"
#include "fatfs_sd.h"

#include "solution.h"
#include "queue.h"
#include "semphr.h"

#include "random_string.h"
#include "ds18b20.h"

#define PERIOD			1000

const char commands[4] = COMMANDS;
const char *archivesRead[10] = ARCHIVES_READ;
const char *archivesWrite[10] = ARCHIVES_WRITE;

float temp = 0;

/* FAT FS */
FATFS fs;
FIL fil;
UINT br, bw;
FRESULT fresult;

/* Task Variables */
TaskHandle_t xTaskHandle0 = NULL;
TaskHandle_t xTaskHandle1 = NULL;
TaskHandle_t xTaskHandle2 = NULL;
TaskHandle_t xTaskHandle3 = NULL;

/* Queue Command Variables */
QueueHandle_t xCommandWRITE = NULL;
QueueHandle_t xCommandREAD = NULL;

/* Queue Archives Variables */
QueueHandle_t xArchivesWRITE = NULL;
QueueHandle_t xArchivesREAD = NULL;

/* Queue Finished Variables */
QueueHandle_t xFinished = NULL;

/* Semaphore Archive Variables */
SemaphoreHandle_t xArch0 = NULL;
SemaphoreHandle_t xArch1 = NULL;
SemaphoreHandle_t xArch2 = NULL;
SemaphoreHandle_t xArch3 = NULL;
SemaphoreHandle_t xArch4 = NULL;
SemaphoreHandle_t xArch5 = NULL;
SemaphoreHandle_t xArch6 = NULL;
SemaphoreHandle_t xArch7 = NULL;
SemaphoreHandle_t xArch8 = NULL;
SemaphoreHandle_t xArch9 = NULL;

void runSolution(void) {

	/* Microprocessor stuck if detect mount SD error */
	if(!connected_sd_card()) while(1);

	/* Configuring register time */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	/* Configuring random values */
	prvConfigRand();

	/* Creating Queues for commands */
	xCommandWRITE = xQueueCreate(10, sizeof(char));
	xCommandREAD = xQueueCreate(10, sizeof(char));

	/* Creating Queues for archives */
	xArchivesWRITE = xQueueCreate(10, (sizeof(char) * 5));
	xArchivesREAD = xQueueCreate(10, (sizeof(char) * 5));

	/* Creating Queues for finished commands */
	xFinished = xQueueCreate(10, sizeof(char));


	/* Creating mutexes for files */
	xArch0 = xSemaphoreCreateMutex();
	xArch1 = xSemaphoreCreateMutex();
	xArch2 = xSemaphoreCreateMutex();
	xArch3 = xSemaphoreCreateMutex();
    xArch4 = xSemaphoreCreateMutex();
	xArch5 = xSemaphoreCreateMutex();
	xArch6 = xSemaphoreCreateMutex();
	xArch7 = xSemaphoreCreateMutex();
	xArch8 = xSemaphoreCreateMutex();
	xArch9 = xSemaphoreCreateMutex();

	/* Creating Tasks */
	xTaskCreate(v1Task0_Handler, "Task-1", (configMINIMAL_STACK_SIZE * 01), NULL, 6, &xTaskHandle0);
	xTaskCreate(v1Task1_Handler, "Task-2", (configMINIMAL_STACK_SIZE * 05), NULL, 5, &xTaskHandle1);
	xTaskCreate(v1Task2_Handler, "Task-3", (configMINIMAL_STACK_SIZE * 05), NULL, 4, &xTaskHandle2);
	xTaskCreate(v1Task3_Handler, "Task-4", (configMINIMAL_STACK_SIZE * 01), NULL, 3, &xTaskHandle3);

	/* Starting the Scheduler */
	vTaskStartScheduler();
}

char prvSendCommand();
char prvGETCommand();
char* prvGETArchiveRead();
char* prvGETArchiveWrite();
void commandCompleted();
void commandFailed();
void readBlock(char *archive);
int bufsize (char *buf);
void writeArchive(char *archive);

/*
 *
 *   This task check if exists one command from single processor.
 *   If exists, it send a command for respective tasks to process a file.
 *   If not, it will sleep again until the next job activation.
 *
 *   TO DO:
 * 		  Deixar mais rígido. Aguardar a flag de sincronização pra receber outro comando.
 *
 *
 * */

void v1Task0_Handler(void *params) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = PERIOD;
	xLastWakeTime = xTaskGetTickCount();

	char command, response, *archive;
	int send_response = 0;

	for (;;) {
		vTaskDelayUntil(&xLastWakeTime, xFrequency);

		/* Send commands */
		command = prvGETCommand();
		if (command != 'N') {
			if (command == 'R') {
				archive = prvGETArchiveRead();
				send_response = xQueueSend(xCommandREAD, &command, pdMS_TO_TICKS(0)) == pdPASS ? 1 : 0;
				send_response == 1 ? xQueueSend(xArchivesREAD, &archive, pdMS_TO_TICKS(0)): commandFailed();
			}else if(command == 'W'){
				archive = prvGETArchiveWrite();
				send_response = xQueueSend(xCommandWRITE, &command, pdMS_TO_TICKS(0)) == pdPASS ? 1 : 0;
				send_response == 1 ? xQueueSend(xArchivesWRITE, &archive, pdMS_TO_TICKS(0)): commandFailed();
			}
		}

		/* Check queue finished */
		xQueueReceive(xFinished, &response, pdMS_TO_TICKS(0));
		if (response == 'R' || response == 'W' || response == 'A') {
			commandCompleted();
		} else if(response == 'F'){
			commandFailed();
		}
	}
}

void v1Task1_Handler(void *params){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = PERIOD;
	xLastWakeTime = xTaskGetTickCount();

	char c, *archive;

	for (;;) {
		vTaskDelayUntil(&xLastWakeTime, xFrequency);

		if (xQueueReceive(xCommandREAD, &c, pdMS_TO_TICKS(0)) == pdPASS) {
			xQueueReceive(xArchivesREAD, &archive, pdMS_TO_TICKS(0));


			if (strcmp(archivesRead[0], archive) == 0) { 		// Read Archive 0
				xSemaphoreTake(xArch0, portMAX_DELAY);
				readBlock(archive);
				xQueueSend(xFinished, &c, pdMS_TO_TICKS(0));
				xSemaphoreGive(xArch0);
			} else if (strcmp(archivesRead[1], archive) == 0) { // Read Archive 1
				xSemaphoreTake(xArch1, portMAX_DELAY);
				readBlock(archive);
				xQueueSend(xFinished, &c, pdMS_TO_TICKS(0));
				xSemaphoreGive(xArch1);
			} else if (strcmp(archivesRead[2], archive) == 0) { // Read Archive 2
				xSemaphoreTake(xArch2, portMAX_DELAY);
				readBlock(archive);
				xQueueSend(xFinished, &c, pdMS_TO_TICKS(0));
				xSemaphoreGive(xArch2);
			} else if (strcmp(archivesRead[3], archive) == 0) { // Read Archive 3
				xSemaphoreTake(xArch3, portMAX_DELAY);
				readBlock(archive);
				xQueueSend(xFinished, &c, pdMS_TO_TICKS(0));
				xSemaphoreGive(xArch3);
			} else if (strcmp(archivesRead[4], archive) == 0) { // Read Archive 4
				xSemaphoreTake(xArch4, portMAX_DELAY);
				readBlock(archive);
				xQueueSend(xFinished, &c, pdMS_TO_TICKS(0));
				xSemaphoreGive(xArch4);
			} else if (strcmp(archivesRead[5], archive) == 0) { // Read Archive 5
				xSemaphoreTake(xArch5, portMAX_DELAY);
				readBlock(archive);
				xQueueSend(xFinished, &c, pdMS_TO_TICKS(0));
				xSemaphoreGive(xArch5);
			} else if (strcmp(archivesRead[6], archive) == 0) { // Read Archive 6
				xSemaphoreTake(xArch6, portMAX_DELAY);
				readBlock(archive);
				xQueueSend(xFinished, &c, pdMS_TO_TICKS(0));
				xSemaphoreGive(xArch6);
			} else if (strcmp(archivesRead[7], archive) == 0) { // Read Archive 7
				xSemaphoreTake(xArch7, portMAX_DELAY);
				readBlock(archive);
				xQueueSend(xFinished, &c, pdMS_TO_TICKS(0));
				xSemaphoreGive(xArch7);
			} else if (strcmp(archivesRead[8], archive) == 0) { // Read Archive 8
				xSemaphoreTake(xArch8, portMAX_DELAY);
				readBlock(archive);
				xQueueSend(xFinished, &c, pdMS_TO_TICKS(0));
				xSemaphoreGive(xArch8);
			} else if (strcmp(archivesRead[9], archive) == 0) { // Read Archive 9
				xSemaphoreTake(xArch9, portMAX_DELAY);
				readBlock(archive);
				xQueueSend(xFinished, &c, pdMS_TO_TICKS(0));
				xSemaphoreGive(xArch9);
			}
		}
	}
}

void v1Task2_Handler(void *params) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = PERIOD;
	xLastWakeTime = xTaskGetTickCount();
	char c, *archive;

	for (;;) {
		vTaskDelayUntil(&xLastWakeTime, xFrequency);

		if (xQueueReceive(xCommandWRITE, &c, pdMS_TO_TICKS(0)) == pdPASS) {

			xQueueReceive(xArchivesWRITE, &archive, pdMS_TO_TICKS(0));

			if (strcmp(archivesRead[0], archive) == 0) { 		// Read Archive 0
				xSemaphoreTake(xArch0, portMAX_DELAY);
				writeArchive(archive);
				xSemaphoreGive(xArch0);
			} else if (strcmp(archivesRead[1], archive) == 0) { // Read Archive 1
				xSemaphoreTake(xArch1, portMAX_DELAY);
				writeArchive(archive);
				xSemaphoreGive(xArch1);
			} else if (strcmp(archivesRead[2], archive) == 0) { // Read Archive 2
				xSemaphoreTake(xArch2, portMAX_DELAY);
				writeArchive(archive);
				xSemaphoreGive(xArch2);
			} else if (strcmp(archivesRead[3], archive) == 0) { // Read Archive 3
				xSemaphoreTake(xArch3, portMAX_DELAY);
				writeArchive(archive);
				xSemaphoreGive(xArch3);
			} else if (strcmp(archivesRead[4], archive) == 0) { // Read Archive 4
				xSemaphoreTake(xArch4, portMAX_DELAY);
				writeArchive(archive);
				xSemaphoreGive(xArch4);
			} else if (strcmp(archivesRead[5], archive) == 0) { // Read Archive 5
				xSemaphoreTake(xArch5, portMAX_DELAY);
				writeArchive(archive);
				xSemaphoreGive(xArch5);
			} else if (strcmp(archivesRead[6], archive) == 0) { // Read Archive 6
				xSemaphoreTake(xArch6, portMAX_DELAY);
				writeArchive(archive);
				xSemaphoreGive(xArch6);
			} else if (strcmp(archivesRead[7], archive) == 0) { // Read Archive 7
				xSemaphoreTake(xArch7, portMAX_DELAY);
				writeArchive(archive);
				xSemaphoreGive(xArch7);
			} else if (strcmp(archivesRead[8], archive) == 0) { // Read Archive 8
				xSemaphoreTake(xArch8, portMAX_DELAY);
				writeArchive(archive);
				xSemaphoreGive(xArch8);
			} else if (strcmp(archivesRead[9], archive) == 0) { // Read Archive 9
				xSemaphoreTake(xArch9, portMAX_DELAY);
				writeArchive(archive);
				xSemaphoreGive(xArch9);
			}
		}
	}
}

void v1Task3_Handler(void *params) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = PERIOD;
	xLastWakeTime = xTaskGetTickCount();

	for (;;) {
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		temp = DS18B20_Celsius();
	}
}


char prvGETCommand() {
	for(int i = 0; i < 30; i++);
	return (char) commands[(rand() % 3)];
}

char* prvGETArchiveRead(){
	for(int i = 0; i < 100; i++);
	return (char*) archivesRead[(rand() % 10)];
}

char* prvGETArchiveWrite(){
	for(int i = 0; i < 100; i++);
	return (char*) archivesRead[(rand() % 10)];
}

void commandCompleted() {
	for (int i = 0; i < 50; i++);
}

void commandFailed(){
	for (int i = 0; i < 100; i++);
}

int sizeArchive(char *archive) {
	int size = 0;
	if (f_open(&fil, archive, FA_READ) == FR_OK) {
		size = (fil).obj.objsize;
		f_close(&fil);
	}
	return size;
}

void readBlock(char *archive) {
	int readed = 0, size = 0, parts = 0;
	if (f_open(&fil, archive, FA_READ) == FR_OK) {
		readed = 0;
		size = (fil).obj.objsize;

		if (size < 10000) {
			parts = size / 10;
		} else if (size > 1000 && size < 20000) {
			parts = size / 15;
		} else {
			parts = size / 20;
		}

		char line[parts];

		while (readed <= size) {
			f_gets(line, sizeof line, &fil);
			readed = readed + parts;
		}

		f_close(&fil);
	} else {
		char failed = 'F';
		xQueueSend(xFinished, &failed, pdMS_TO_TICKS(0));
	}
}

void writeArchive(char *archive) {
	char *stringFromProcessor = cRandomString(20 + (DWT->CYCCNT % 80));
	int size = sizeArchive(archive);

	if (size != 0 && size < 30000) {
		fresult = f_open(&fil, archive, FA_OPEN_EXISTING | FA_READ | FA_WRITE);
		if (fresult == FR_OK) {
			fresult = f_lseek(&fil, f_size(&fil));
			if (fresult == FR_OK) {
				fresult = f_puts(stringFromProcessor, &fil);
			}
		} else {
			char failed = 'F';
			xQueueSend(xFinished, &failed, pdMS_TO_TICKS(0));
		}
		f_close(&fil);
	} else if (size != 0) {
		fresult = f_unlink(archive);
		f_close(&fil);
		if (fresult == FR_OK) {
			fresult = f_open(&fil, archive, FA_CREATE_ALWAYS | FA_WRITE);
			if (fresult == FR_OK) {
				fresult = f_write(&fil, stringFromProcessor, bufsize(stringFromProcessor), &bw);
				if (fresult == FR_OK) {
					char not_failed = 'C';
					xQueueSend(xFinished, &not_failed, pdMS_TO_TICKS(0));
				} else {
					char failed = 'F';
					xQueueSend(xFinished, &failed, pdMS_TO_TICKS(0));
				}
			} else {
				char failed = 'F';
				xQueueSend(xFinished, &failed, pdMS_TO_TICKS(0));
			}
			f_close(&fil);
		} else {
			char failed = 'F';
			xQueueSend(xFinished, &failed, pdMS_TO_TICKS(0));
		}
	}
	vPortFree(stringFromProcessor);
}

int bufsize (char *buf)
{
	int i=0;
	while (*buf++ != '\0') i++;
	return i;
}

