#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ALab.h"

list_xor_t* Xor(list_xor_t* prev, list_xor_t* next) {
	list_xor_t* current;

	current = (list_xor_t*)((intptr_t)prev ^ (intptr_t)next);
	return current;
}

list_xor_t* CreateList() {
	list_xor_t* element = (list_xor_t*)(malloc(sizeof(list_xor_t)));

	if (element == NULL) {
		return NULL;
	}

	element->data = NULL;
	element->xor = NULL;

	return element;
}

list_xor_t* Push(list_xor_t* head, char* str) {
	list_xor_t* newElement = (list_xor_t*)malloc(sizeof(list_xor_t));

	if (newElement == NULL) {
		return NULL;
	}
	else {
		newElement->data = str;
		newElement->xor = head;
		newElement->xor = Xor(head, NULL);

		if (head != NULL) {
			head->xor = Xor(head->xor, newElement);
		}
		else {
			return NULL;
		}

		return newElement;
	}
}

void DeletElemKey(list_xor_t* head, char* key) {
	list_xor_t* current = head;
	list_xor_t* previous = NULL;
	list_xor_t* next;

	current = FindByKey(key, current, &previous);

	if (current != NULL) {
		next = Xor(current->xor, previous);

		if (previous != NULL) {
			previous->xor = Xor(Xor(previous->xor, current), next);
		}
		if (next != NULL) {
			next->xor = Xor(Xor(next->xor, current), previous);
		}

		free(current);
	}
}

void DeletElemAddress(list_xor_t* head, list_xor_t* pointer) {
	list_xor_t* current = head;
	list_xor_t* previous = NULL;
	list_xor_t* next;

	while (current != pointer) {

		if (current->xor == previous && current != head) {
			return;
		}

		if (current-> xor == NULL) {
			return;
		}
		else {
			Iterating(&current, &previous);
		}

	}

	if (current != NULL) {
		next = Xor(current->xor, previous);

		if (previous != NULL) {
			previous->xor = Xor(Xor(previous->xor, current), next);
			printf("POPAL 7\n");
		}
		if (next != NULL) {
			next->xor = Xor(Xor(next->xor, current), previous);
			printf("POPAL 8\n");
		}

		free(current);
	}
	else {
		return;
	}
}

list_xor_t* FindByKey(char* key, list_xor_t* head, list_xor_t** previous) {
	list_xor_t* current = head;
	int i = 0;

	while (current != NULL) {
		if (current->data == NULL){
			Iterating(&current, previous);
		}
		else if ((int)strcmp(current->data, key) != 0) {
			Iterating(&current, previous);
		}
		else {
			i++;
			break;
		}
	}

	if (i == 0) {
		return NULL;
	}
	else {
		return current;
	}
}

void Iterating(list_xor_t** current, list_xor_t** previous) {
	list_xor_t* next;

	next = Xor(*previous, (*current)->xor);
	*previous = *current;
	*current = next;
}

void DeleteList(list_xor_t* head) {
	list_xor_t* current = head->xor;
	list_xor_t* p;

	while (current != NULL) {
		p = head;
		Iterating(&current, &head);
		free(p);
	}
	free(head);
}