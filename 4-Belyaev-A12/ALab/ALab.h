#pragma once
#ifndef LabA_H_INCLUDED_
#define LabA_H_INCLUDED_

typedef struct list_xor_t {
	char* data;
	struct list_xor_t*xor;
} list_xor_t;

list_xor_t* Xor(list_xor_t* prev, list_xor_t* next);//++
list_xor_t* CreateList(void);//++
list_xor_t* Push(list_xor_t** head, char* str);//++
void DeletElemKey(list_xor_t* head, char* key);//++
void DeletElemAddress(list_xor_t* head, list_xor_t* pointer);//++
list_xor_t* FindByKey(char* key, list_xor_t* head, list_xor_t** previous);//++
void Iterating(list_xor_t** current, list_xor_t** previous);//++
void DeleteList(list_xor_t* head);//++

#endif