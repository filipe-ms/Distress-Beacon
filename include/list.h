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

// Cria uma lista genérica.
List* List_Create(size_t data_size);

// Adiciona no começo.
int List_Add(List* list, void* data);

// Adiciona no fim.
int List_AddLast(List* list, void* data);

// Remove um nó específico.
int List_Remove(List* list, void* data);

// Remove um nó em determinada posição.
int List_RemoveAt(List* list, int index);

// Remove o primeiro nó da lista.
int List_RemoveFirst(List* list);

// Destrói a lista e libera os dados.
// List fica NULL depois disso!
int List_Destroy(List* target);

// Libera todas as listas criadas.
// Libera também os dados de cada nó.
int List_FreeAll(void);

// Pego um elemento em um index da lista.
void* List_GetByIndex(List* list, int index);


// Parte funcional:

// Aplica uma função a cada elemento da lista.
void List_ForEach(List* list, Function arg);

// Aplica uma função a cada elemento da lista. Aceita 2 argumentos.
void List_ForEachCtx(List* list, void* context, TwoArgFunction fn);

// Encontra um elemento na lista usando um predicado (função de correspondência).
void* List_FindWithFn(List* list, void* context, MatchFunction matchfn);

// Adiciona um elemento na lista em ordem, usando uma função de comparação.
int List_AddWithFn(List* list, void* data, ComparisonFunc comparefn);

// Remove um elemento da lista usando uma função de correspondência.
int List_RemoveWithFn(List* list, void* context, MatchFunction matchfn);