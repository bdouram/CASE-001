#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "random_string.h"

#include "stm32f4xx.h"

#include "FreeRTOS.h"

void prvConfigRand(void) {

	int *y;
	int b = *y;

	if (b < 0) {
		b = b * -1;
	}

	srand(b);
}

static char *rand_string(char *str, int size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%&*()+/-.^<>;?";
    if (size) {
        --size;
        for (int n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char* cRandomString(int size)
{
     char *s = pvPortMalloc(size + 1);
     if (s) {
         rand_string(s, size);
     }
     return s;
}
