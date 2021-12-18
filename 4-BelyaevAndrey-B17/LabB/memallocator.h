#pragma once
#include <stdio.h> 
#include <stdint.h>
#include <string.h>

#define HEADER(pointer) (list_t*)((uint8_t)poiner-sizeof(list_t))
#define FROM_HEAD 0
#define FROM_TAIL 1

typedef struct list {
	struct list* next;
	struct list* prev;
	int size;
}list_t;


