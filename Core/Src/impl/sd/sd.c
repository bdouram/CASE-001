#include <string.h>
#include "stm32f4xx.h"
#include "fatfs.h"
#include "fatfs_sd.h"
#include "sd.h"

FATFS fs;
FIL fil; // File
UINT br, bw;

int size_block = 1;

int connected_sd_card(void) {
	return f_mount(&fs, "/", 1) == FR_OK ? 1 : 0;
}
