#include <stdio.h> 
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "memallocator.h"
#define FROM_HEAD 0
#define FROM_TAIL 1
#define FALSE 0
#define TRUE 1

typedef struct list {
	struct list* next;
	struct list* prev;
	uint32_t bSize;
	uint8_t Used;
}list_t;

list_t* listHead = NULL;
list_t* listTail = NULL;
uint32_t SIZE = 0;
uint8_t flag = 1;


list_t* getLastBlock() {
	list_t* finder = listHead;
	while (finder->next != NULL) {
		finder = finder->next;
	}

	return finder;
}

void memBlockCreate(list_t* block, uint32_t size) {
	if (block->bSize <= size)
		return;

	if (block->bSize - size < sizeof(list_t)) {
		return;
	}
	list_t* newBlock = (list_t*)((uint8_t*)block + sizeof(list_t) + sizeof(uint8_t) * size);
	newBlock->next = block->next;
	newBlock->Used = FALSE;
	newBlock->prev = block;
	newBlock->bSize = block->bSize - size - sizeof(list_t);

	if (block->next != NULL)
		block->next->prev = newBlock;

	block->next = newBlock;
	block->bSize = size;
}


void* memalloc(int size) {
	if (size <= 0 || !listHead) {
		return NULL;
	}

	list_t* pointer = NULL;
	list_t* current = listHead;
	list_t* neoPointer = NULL;
	int blockSize = 0;
	void* ptr;

	if (flag) {
		flag = 1;
		list_t* fromFirst = listHead;

		while (fromFirst != NULL) {
			if (!fromFirst->Used && fromFirst->bSize >= size) {
				memBlockCreate(fromFirst, size);
				fromFirst->Used = TRUE;
				return (void*)((uint8_t*)fromFirst + sizeof(list_t));
			}
			fromFirst = fromFirst->next;
		}
	}
	else {
		flag = 0;
		list_t* fromLast = getLastBlock();

		while (fromLast != NULL) {
			if (!fromLast->Used && fromLast->bSize >= size) {
				memBlockCreate(fromLast, size);
				fromLast->Used = TRUE;
				return (void*)((uint8_t*)fromLast + sizeof(list_t));
			}
			fromLast = fromLast->prev;
		}
	}

	return NULL;
}

uint8_t mergeCheck(list_t* left, list_t* right) {
	if (left == NULL || right == NULL) {
		return FALSE;
	}
	uint32_t size;
	if (left->bSize == 0) {
		size = 1;
	}

	else {
		size = left->bSize;
	}

	if (left->bSize > 0) {
		if ((uint8_t*)left + sizeof(list_t) + sizeof(uint8_t) * size == (uint8_t*)right) {
			return TRUE;
		}
	}

	return FALSE;
}

int8_t memBlockMerge(list_t* left, list_t* right) {
	if (left == NULL || right == NULL)
		return FALSE;

	if (!left->Used && !right->Used)
		if (mergeCheck(left, right)) {
			if (right->next != NULL)
				right->next->prev = left;
			left->next = right->next;
			left->bSize += right->bSize + sizeof(list_t);

			return TRUE;
		}
	return FALSE;

}

void memfree(void* p) {
	if (p == NULL) {
		return;
	}

	list_t* free = (list_t*)((uint8_t*)p - sizeof(list_t));

	free->Used = FALSE;

	if (free->prev != NULL) {
		if (memBlockMerge(free->prev, free)) {
			free = free->prev;
		}
	}

	if (free->next != NULL) {
		memBlockMerge(free, free->next);
	}
}


int meminit(void* pMemory, int size) {
	if (pMemory == NULL || (listHead != NULL) || size <= sizeof(list_t)) {
		return 0;
	}

	listHead = (list_t*)pMemory;
	listHead->bSize = size - sizeof(list_t);
	listHead->next = NULL;
	listHead->prev = NULL;
	listHead->Used = NULL;

	SIZE = size;
	return 1;
}

int memgetminimumsize() {
	return sizeof(list_t);
}

void memdone() {
	list_t* head = listHead;
	uint32_t fullSize = 0;
	while (head != NULL) {
		fullSize += head->bSize + sizeof(list_t);
		head = head->next;
	}

	listHead = NULL;
}

int memgetblocksize() {
	return sizeof(list_t);
}

