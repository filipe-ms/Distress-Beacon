// File: list.h
#pragma once

typedef void (*Function)(void* arg);
typedef void (*TwoArgFunction)(void* arg1, void* arg2);
typedef int (*MatchFunction)(void* context, void* item);
typedef int (*ComparisonFunc)(void* item1, void* item2);

#include <stddef.h>

typedef struct Node {
	void* data;
	struct Node* next;
} Node;

typedef struct List {
	Node* head;
	Node* tail;
	int size;
	size_t data_size;
} List;

// Cria uma lista gen�rica.
List* List_Create(size_t data_size);

// Adiciona no come�o.
int List_Add(List* list, void* data);

// Adiciona no fim.
int List_AddLast(List* list, void* data);

// Remove um n� espec�fico.
int List_Remove(List* list, void* data);

// Remove um n� em determinada posi��o.
int List_RemoveAt(List* list, int index);

// Remove o primeiro n� da lista.
int List_RemoveFirst(List* list);

// Destr�i a lista e libera os dados.
// List fica NULL depois disso!
int List_Destroy(List* target);

// Libera todas as listas criadas.
// Libera tamb�m os dados de cada n�.
int List_FreeAll(void);

// Pego um elemento em um index da lista.
void* List_GetByIndex(List* list, int index);


// Parte funcional:

// Aplica uma fun��o a cada elemento da lista.
void List_ForEach(List* list, Function arg);

// Aplica uma fun��o a cada elemento da lista. Aceita 2 argumentos.
void List_ForEachCtx(List* list, void* context, TwoArgFunction fn);

// Encontra um elemento na lista usando um predicado (fun��o de correspond�ncia).
void* List_FindWithFn(List* list, void* context, MatchFunction matchfn);

// Adiciona um elemento na lista em ordem, usando uma fun��o de compara��o.
int List_AddWithFn(List* list, void* data, ComparisonFunc comparefn);

// Remove um elemento da lista usando uma fun��o de correspond�ncia.
int List_RemoveWithFn(List* list, void* context, MatchFunction matchfn);