#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h> 
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "memallocator.h"
#define FROM_HEAD 0
#define FROM_TAIL 1

typedef struct list {
	struct list* next;
	struct list* prev;
	int size;
}list_t;

list_t* listHead = NULL;
list_t* listTail = NULL;
int SIZE = 0;
int flag;


void* memalloc(int size) {
	if (size <= 0 || !listHead) {
		return NULL;
	}

	list_t* pointer = NULL;
	list_t* current = listHead;
	list_t* neoPointer = NULL;
	int blockSize = 0;
	void* ptr;

	if (flag == FROM_TAIL) {
		current = listTail;
	}

	while (current) {
		if ((size <= abs(current->size)) && (current->size < 0)) {
			pointer = current;
			blockSize = -(current->size);

			if (blockSize - size > sizeof(list_t)) {
				neoPointer = (list_t*)((char*)pointer + size + sizeof(list_t));
				neoPointer->prev = pointer;
				neoPointer->next = pointer->next;
				neoPointer->size = -((int)blockSize) + size + sizeof(list_t);

				if (pointer->next) {
					pointer->next->prev = neoPointer;
				}

				pointer->next = neoPointer;
				pointer->size = size;

				if (neoPointer->next == NULL) {
					listTail = neoPointer;
				}
			}
			else {
				pointer->size = -(pointer->size);
				if (pointer->next == NULL) {
					listTail = pointer;
				}
			}

			ptr = (void*)((char*)pointer + sizeof(list_t));

			return ptr;
		}

		if (flag == FROM_TAIL) {
			current = current->prev;
		}
		else {
			current = current->next;
		}
	}
	if (flag == FROM_TAIL) {
		flag = FROM_HEAD;
	}
	else {
		flag = FROM_TAIL;
	}

	return NULL;
}


void memfree(void* pointer) {
	list_t* free = NULL;

	if (!pointer || listHead) {
		return;
	}

	free = (list_t*)((char*)pointer - sizeof(list_t));

	if (!((listHead <= pointer) && (free < (listHead + SIZE - sizeof(list_t))))) {
		return;
	}


	if (free->size > 0) {

		free->size = -(free->size);

		if (free->prev) {

			if (free->prev->size < 0) {
				list_t* left = NULL;
				left = free->prev;

				left->size = left->size + (free->size - sizeof(list_t));

				if (left->next->next != NULL) {
					left->next->next->prev = left;
					left->next = left->next->next;
				}
				else {
					left->next = NULL;
					listTail = left;
				}

				free = left;

				if (!(free->prev)) {
					listHead = free;
				}

				pointer = NULL;
			}
		}
		else {
			listHead = free;
		}

		if (free->next) {
			if (free->next->size < 0) {
				list_t* right = NULL;
					right = free->next;

				free->size = free->size + free->next->size - sizeof(list_t);

				if (free->next->next) {
					free->next->next->prev = free;
					free->next = free->next->next;
				}
				else {
					free->next = NULL;
					listTail = free;
				}
				if (free->prev) {
					free->prev->next = free;
				}
				else {
					listHead = free;
				}
			}

			if (pointer) {
				pointer = NULL;
			}
		}
		else {
			listTail = free;
		}
	}

}

int meminit(void* pMemory, int size) {
	if ((size <= sizeof(list_t)) || (!pMemory)) {
		return 0;
	}

	listHead = (list_t*)pMemory;
	listHead->size = -(size - memgetblocksize());
	listHead->next = NULL;
	listHead->prev = NULL;
	listTail = listHead;

	SIZE = size;
	return 1;
}

int memgetminimumsize() {
	return sizeof(list_t);
}

void memdone() {
	if (listHead)
	{
		list_t* current = listHead;
		while (current)
		{
			if (current->size > 0)
			{
				break;
			}
			current = current->next;
		}

		current = NULL;
	}
	listHead = NULL;
	SIZE = 0;
}

int memgetblocksize() {
	return sizeof(list_t);
}

#ifdef __cplusplus
}
#endif