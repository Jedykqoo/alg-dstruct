#include "gtest\gtest.h"
extern "C" {
#include "..\ALab\Alab.h"
}
#pragma warning(disable : 6011)


TEST(XorOperation_test, Xor_Idempotency_returnValue) {//a^0=a
	list_xor_t element;

	element.xor = Xor(&element, NULL);
	EXPECT_EQ(element.xor, &element);
}

TEST(XorOperation_test, Xor_SelfReversal_returnValue) {// a^a=0
	list_xor_t element;
	list_xor_t* link1, * link2;

	link1 = Xor(&element, &element);
	link2 = (list_xor_t*)0;

	EXPECT_EQ(link1, link2);
}

TEST(XorOperation_test, Xor_Commutativity_returnValue) {// a^b=b^a
	list_xor_t one;
	list_xor_t two;
	list_xor_t* link1, * link2;

	link1 = Xor(&one, &two);
	link2 = Xor(&two, &one);

	EXPECT_EQ(link1, link2);
}

TEST(XorOperation_test, Xor_Associativity_returnValue) {// (a^b)^c=a^(b^c)
	list_xor_t one;
	list_xor_t two;
	list_xor_t three;
	list_xor_t* link1, * link2;

	link1 = Xor(Xor(&one, &two), &three);
	link2 = Xor(&one, Xor(&two, &three));

	EXPECT_EQ(link1, link2);
}

TEST(XorOperation_test, Xor_Reversibility_returnValue) {// (a^b)^b=a
	list_xor_t one;
	list_xor_t two;
	list_xor_t* link;

	link = Xor(Xor(&one, &two), &two);

	EXPECT_EQ(link, &one);
}


TEST(ListCreation_test, CreateList_returnPointerToFirstElement) {
	list_xor_t* head;

	head = CreateList();

	EXPECT_TRUE(head);
	free(head);
}


TEST(DeletingAList_test, DeleteList_returnPointerToFirstElement) {
	list_xor_t* element1 = (list_xor_t*)malloc(sizeof(list_xor_t));
	list_xor_t* element2 = (list_xor_t*)malloc(sizeof(list_xor_t));
	list_xor_t* head = element1;

	element1->data = "test 1";
	element1->xor = element2;
	element2->data = "test 2";
	element2->xor = element1;

	DeleteList(head);

	EXPECT_FALSE(element1->data == "test 1");
	EXPECT_FALSE(element2->data == "test 2");
}


TEST(AddElementToList_test, Push_Creation1Element_returnPointerToTheAddedElement) {
	list_xor_t* element1 = (list_xor_t*)malloc(sizeof(list_xor_t));
	list_xor_t* head = element1;
	list_xor_t* element2;
	char* str = "test 2";

	element1->xor = NULL;
	element1->data = "test 1";

	element2 = Push(element1, str);

	EXPECT_FALSE(head == element2);
	EXPECT_FALSE(element2-> xor == NULL);
	EXPECT_EQ(element2->data, str);

	free(element1);
	free(element2);
}

TEST(AddElementToList_test, Push_Creation2Element_returnPointerToTheAddedElement) {
	list_xor_t* element1 = (list_xor_t*)malloc(sizeof(list_xor_t));
	list_xor_t* element2, * element3;
	list_xor_t* head = element1;
	char* str2 = "test 2", *str3 = "test 3";

	element1->data = NULL;
	element1->xor = NULL;

	element2 = Push(element1, str2);
	element3 = Push(element2, str3);

	EXPECT_FALSE(head == element2);
	EXPECT_FALSE(head == element3);
	EXPECT_FALSE(element2 == element3);
	EXPECT_FALSE(element2 == NULL);
	EXPECT_FALSE(element3 == NULL);

	EXPECT_EQ(element1->xor, element2);
	EXPECT_EQ(element2->xor, (list_xor_t*)((intptr_t)element1 ^ (intptr_t)element3));
	EXPECT_EQ(element3->xor, element2);
	EXPECT_EQ(element2->data, str2);
	EXPECT_EQ(element3->data, str3);

	free(element1);
	free(element2);
	free(element3);
}

TEST(AddElementToList_test, Push_NullHeadPointer_returnPointerToTheAddedElement) {
	list_xor_t* element1 = NULL;
	list_xor_t* element2;
	char* str = "test 2";

	element2 = Push(element1, str);

	EXPECT_TRUE(element2 == NULL);

	free(element1);
	free(element2);
}


TEST(Iteration_test, Iterating_EmptyList_returnCorrectValue) {
	list_xor_t list;
	list.xor = NULL;
	list_xor_t* p1, * p2;
	p1 = NULL;
	p2 = &list;

	Iterating(&p2, &p1);

	EXPECT_FALSE(p2);
}

TEST(Iteration_test, Iterating_AtTheBeginningList_returnCorrectValue) {
	list_xor_t element1, element2;
	element1.xor = &element2;
	element2.xor = &element1;
	list_xor_t* head = &element1;
	list_xor_t* prevHead = NULL;

	Iterating(&head, &prevHead);

	EXPECT_EQ(head, &element2);
	EXPECT_EQ(prevHead, &element1);
}

TEST(Iteration_test, Iterating_AtTheMiddleList_returnCorrectValue) {
	list_xor_t p1, p2, p3;
	list_xor_t* p2_p, * head;
	p2_p = &p2;
	head = &p1;
	p1.xor = &p2;
	p2.xor = (list_xor_t*)((intptr_t)(&p1) ^ (intptr_t)(&p3));
	p3.xor = &p2;

	Iterating(&p2_p, &head);

	EXPECT_EQ(p2_p, &p3);
	EXPECT_EQ(head, &p2);
}

TEST(Iteration_test, Iterating_AtTheEndList_returnCorrectValue) {
	list_xor_t p1, p2;
	list_xor_t* p2_p, * head;
	p2_p = &p2;
	head = &p1;
	p1.xor = &p2;
	p2.xor = &p1;

	Iterating(&p2_p, &head);

	EXPECT_FALSE(p2_p);
	EXPECT_EQ(head, &p2);
}


TEST(FindElement_test, FindByKey_EmptyList_returnNULL) {
	list_xor_t element = { NULL, NULL };
	list_xor_t* foundElement = NULL, *prev = NULL;
	char* str = "test";

	foundElement = FindByKey(str, &element, &prev);

	EXPECT_TRUE(foundElement == NULL);
}

TEST(FindElement_test, FindByKey_NoKeyMatches_returnNull){
	list_xor_t element1, element2;
	list_xor_t* previous = NULL;
	list_xor_t* foundElement;

	element1.data = "test 1";
	element1.xor = &element2;
	element2.data = "test 2";
	element2.xor = &element1;

	foundElement = FindByKey("test 3" ,&element1, &previous);

	EXPECT_FALSE(foundElement != NULL);
}

TEST(FindElement_test, FindByKey_MatchInTheFirstElement_returnCorrectValue) {
	list_xor_t element1, element2, element3;
	list_xor_t* previous = NULL;
	list_xor_t* foundElement;
	char* str = "test 1";

	element1.data = "test 1";
	element1.xor = &element2;
	element2.data = "test 2";
	element2.xor = (list_xor_t*)((intptr_t)(&element1) ^ (intptr_t)(&element3));
	element3.data = "test 3";
	element3.xor = &element2;

	foundElement = FindByKey("test 1", &element1, &previous);

	EXPECT_EQ(foundElement, &element1);
}

TEST(FindElement_test, FindByKey_MatchInTheMiddleOfTheList_returnCorrectValue){
	list_xor_t element1, element2, element3;
	list_xor_t* previous = NULL;
	list_xor_t* foundElement;
	char* str = "test 2";

	element1.data = "test 1";
	element1.xor = &element2;
	element2.data = "test 2";
	element2.xor = (list_xor_t*)((intptr_t)(&element1) ^ (intptr_t)(&element3));
	element3.data = "test 3";
	element3.xor = &element2;

	foundElement = FindByKey(str, &element1, &previous);

	EXPECT_EQ(foundElement, &element2);
}

TEST(FindElement_test, FindByKey_MatchInTheLastItemInTheList_returnCorrectValue) {
	list_xor_t element1, element2, element3;
	list_xor_t* previous = NULL;
	list_xor_t* foundElement;
	char* str = "test 3";

	element1.data = "test 1";
	element1.xor = &element2;

	element2.data = "test 2";
	element2.xor = (list_xor_t*)((intptr_t)(&element1) ^ (intptr_t)(&element3));

	element3.data = "test 3";
	element3.xor = &element2;

	foundElement = FindByKey(str, &element1, &previous);

	EXPECT_EQ(foundElement, &element3);
}


TEST(DeletingAListItemByKey_test, DeletElemKey_RemovingTheFirstElement_returnCorrectPointers){
	list_xor_t* element1 = (list_xor_t*)malloc(sizeof(list_xor_t));
	list_xor_t element2, element3;
	list_xor_t* head = element1;
	char* str = "test 1";

	element1->xor = &element2;
	element1->data = "test 1";

	element2.xor = (list_xor_t*)((uintptr_t)(element1) ^ (uintptr_t)(&element3));
	element2.data = "test 2";

	element3.xor = &element2;
	element3.data = "test 3";

	DeletElemKey(head, str);

	EXPECT_EQ(element2.xor, &element3);
}

TEST(DeletingAListItemByKey_test, DeletElemKey_RemovingTheMiddleElement_returnCorrectPointers){
	list_xor_t element1, element3;
	list_xor_t* head = &element1;
	list_xor_t* element2 = (list_xor_t*)malloc(sizeof(list_xor_t));
	char* str = "test 2";

	element1.xor = element2;
	element1.data = "test 1";

	element2->xor = (list_xor_t*)((uintptr_t)(&element1) ^ (uintptr_t)(&element3));
	element2->data = "test 2";

	element3.xor = element2;
	element3.data = "test 3";

	DeletElemKey(head, str);

	EXPECT_EQ(element1.xor, &element3);
	EXPECT_EQ(element3.xor, &element1);
}

TEST(DeletingAListItemByKey_test, DeletElemKey_RemovingTheLastElement_returnCorrectPointers){
	list_xor_t element1, element2;
	list_xor_t* head = &element1;
	list_xor_t* element3 = (list_xor_t*)malloc(sizeof(list_xor_t));
	char* str = "test 3";

	element1.xor = &element2;
	element1.data = "test 1";

	element2.xor = (list_xor_t*)((uintptr_t)(&element1) ^ (uintptr_t)(element3));
	element2.data = "test 2";

	element3->xor = &element2;
	element3->data = "test 3";

	DeletElemKey(head, str);

	EXPECT_EQ(element2.xor, &element1);
}

TEST(DeletingAListItemByKey_test, DeletElemKey_NoKeyMatch_returnCorrectPointers) {
	list_xor_t element1, element2, element3;
	list_xor_t* head = &element1, *Xor2;
	char* str = "test 4";

	element1.xor = &element2;
	element1.data = "test 1";

	element2.xor = Xor2 = (list_xor_t*)((uintptr_t)(&element1) ^ (uintptr_t)(&element3));
	element2.data = "test 2";

	element3.xor = &element2;
	element3.data = "test 3";

	DeletElemKey(head, str);

	EXPECT_EQ(element1.xor, &element2);
	EXPECT_STREQ(element1.data, "test 1");
	EXPECT_EQ(element2.xor, Xor2);
	EXPECT_STREQ(element2.data, "test 2");
	EXPECT_EQ(element3.xor, &element2);
	EXPECT_STREQ(element3.data, "test 3");
}

TEST(DeletingAListItemByKey_test, DeletElemKey_EmptyList_returnNULL) {
	list_xor_t element = { NULL, NULL };
	list_xor_t* head = &element;
	char* str = "test";

	DeletElemKey(head, str);

	EXPECT_FALSE(element.data);
	EXPECT_FALSE(element.xor);
}


TEST(DeletingAnElementByAddress_test, DeletElemAddress_RemovingTheFirstElement_returnNullPointer) {
	list_xor_t* element1 = (list_xor_t*)malloc(sizeof(list_xor_t));
	list_xor_t element2, element3;
	list_xor_t* pointer = element1, *head = element1;

	element1->xor = &element2;
	element2.xor = (list_xor_t*)((intptr_t)(element1) ^ (intptr_t)(&element3));
	element3.xor = &element2;

	DeletElemAddress(head, pointer);

	EXPECT_EQ(element2.xor, &element3);
}

TEST(DeletingAnElementByAddress_test, DeletElemAddress_RemovingTheMiddleElement_returnNullPointer) {
	list_xor_t* element2 = (list_xor_t*)malloc(sizeof(list_xor_t));
	list_xor_t element1, element3;
	list_xor_t* pointer = element2, *head = &element1;

	element1.xor = element2;
	element2->xor = (list_xor_t*)((intptr_t)(&element1) ^ (intptr_t)(&element3));
	element3.xor = element2;

	DeletElemAddress(head, pointer);

	EXPECT_EQ(element1.xor, &element3);
	EXPECT_EQ(element3.xor, &element1);
}

TEST(DeletingAnElementByAddress_test, DeletElemAddress_RemovingTheLastElement_returnNullPointer) {
	list_xor_t* element3 = (list_xor_t*)malloc(sizeof(list_xor_t));
	list_xor_t element1, element2;
	list_xor_t* pointer = element3, * head = &element1;

	element1.xor = &element2;
	element2.xor = (list_xor_t*)((intptr_t)(&element1) ^ (intptr_t)(element3));
	element3->xor = &element2;

	DeletElemAddress(head, pointer);

	EXPECT_EQ(element2.xor, &element1);
}

TEST(DeletingAnElementByAddress_test, DeletElemAddress_EmptyList_returnNULL) {
	list_xor_t element = { NULL, NULL };
	list_xor_t* dummyElement = (list_xor_t*)malloc(sizeof(list_xor_t));
	list_xor_t* head = &element, *pointer = dummyElement ;
	list_xor_t* save = pointer;
	char* str = "test";

	DeletElemAddress(head, pointer);

	EXPECT_TRUE(save == pointer);
	EXPECT_TRUE(pointer);
}

TEST(DeletingAnElementByAddress_test, DeletElemAddress_NoPointerMatch_returnCorrectPointers) {
	list_xor_t* dummyElement = (list_xor_t*)malloc(sizeof(list_xor_t));;
	list_xor_t element1, element2, element3;
	list_xor_t* pointer = dummyElement, * head = &element1;
	list_xor_t* save = pointer, * xor2;

	element1.xor = &element2;
	element2.xor = (list_xor_t*)((intptr_t)(&element1) ^ (intptr_t)(&element3));
	element3.xor = &element2;
	printf("POPAL 1\n");
	DeletElemAddress(head, pointer);

	xor2 = (list_xor_t*)((intptr_t)(&element1) ^ (intptr_t)(&element3));

	EXPECT_TRUE(save == pointer);
	EXPECT_TRUE(pointer);
	EXPECT_EQ(element1.xor, &element2);
	EXPECT_EQ(element2.xor, xor2);
	EXPECT_EQ(element3.xor, &element2);

	free(dummyElement);
}