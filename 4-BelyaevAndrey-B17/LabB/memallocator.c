#include "memallocator.h"

list_t* listHead = NULL;
list_t* listTail = NULL;
int SIZE = 0;
int flag;

void* memalloc(int size) {
	if (size <= 0) {
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

	while (current != NULL) {
		if ((size <= current->size) && (current->size < 0)) {
			pointer = current;
			blockSize = -(current->size);

			if (blockSize - size > sizeof(list_t)) {
				neoPointer = (list_t*)(pointer + size + sizeof(list_t));
				neoPointer->prev = pointer;
				neoPointer->next = pointer->next;
				neoPointer->size = (int)blockSize + size + sizeof(list_t);

				if (pointer->next != NULL) {
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

			ptr = (void*)(pointer + sizeof(list_t));

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

	return 0;
}


void mefree(void* pointer) {
	list_t* free = NULL;

	if (pointer == NULL) {
		return;
	}

	free = (list_t*)((int*)pointer - (int)sizeof(list_t));

	if (listHead > free && free < (listHead + SIZE - (int)sizeof(list_t))) {
		return;
	}


	if (free->size > 0) {

		free->size = -(free->size);

		if (free->prev != NULL) {

			if (free->prev->size < 0) {
				list_t* left = free->prev;

				left->size += free->size - sizeof(list_t);

				if (left->next->next != NULL) {
					left->next->next->prev = left;
					left->next = left->next->next;
				}
				else {
					left->next = NULL;
					listTail = left;
				}

				free = left;

				if (free->prev == NULL) {
					listHead = free;
				}

				pointer = NULL;
			}
		}
		else {
			listHead = free;
		}

		if (free->next != NULL) {
			if (free->next->size < 0) {
				list_t* right = free->next;

				free->size = free->size + free->next->size - sizeof(list_t);

				if (free->next->next != NULL) {
					free->next->next->prev = free;
					free->next = free->next->next;
				}
				else {
					free->next = NULL;
					listTail = free;
				}
				if (free->prev != NULL) {
					free->prev->next = free;
				}
				else {
					listHead = free;
				}
			}

			if (pointer != NULL) {
				pointer = NULL;
			}
		}
		else {
			listTail = free;
		}
	}
	else {
		return;
	}
}
