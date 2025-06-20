// File: list.c
#include "list.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Estrutura interna para armazenar todas as listas criadas para a função List_FreeAll
// A ideia é chamar o List_FreeAll ao final do jogo para n�o precisar se lembrar de liberar cada lista individualmente.
typedef struct ListInfo {
    List* list;
    struct ListInfo* next;
} ListInfo;

// Instanciando a lista global de listas criadas
static ListInfo* lists = NULL;

List* List_Create(size_t data_size) {
    List* list = (List*)malloc(sizeof(List));
    if (!list) return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->data_size = data_size;

    ListInfo* new_info = (ListInfo*)malloc(sizeof(ListInfo));
    if (new_info) {
        new_info->list = list;
        new_info->next = lists;
        lists = new_info;
    }
    else {
        free(list);
        return NULL;
    }

    return list;
}

// Adiciona no come�o da lista
int List_Add(List* list, void* data) {
    if (!list || !data) return 0;

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return 0;

    new_node->data = malloc(list->data_size);
    if (!new_node->data) {
        free(new_node);
        return 0;
    }

    memcpy(new_node->data, data, list->data_size);
    new_node->next = list->head;
    list->head = new_node;

    if (!list->tail) {
        list->tail = new_node;
    }

    list->size++;
    return 1;
}

// Adiciona no fim da lista
int List_AddLast(List* list, void* data) {
    if (!list || !data) return 0;

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return 0;

    new_node->data = malloc(list->data_size);
    if (!new_node->data) {
        free(new_node);
        return 0;
    }

    memcpy(new_node->data, data, list->data_size);
    new_node->next = NULL;

    if (list->tail) {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    else {
        list->head = new_node;
        list->tail = new_node;
    }

    list->size++;
    return 1;
}

int List_Remove(List* list, void* data) {
    if (!list || !data || list->size == 0) return 0;

    Node* current = list->head;
    Node* previous = NULL;

    while (current) {
        Node* next = current->next;

        if (current->data == data) {
            if (previous) previous->next = next;
            else list->head = next;

            if (current == list->tail) list->tail = previous;

            free(current->data);
            free(current);
            list->size--;
            return 1;
        }

        previous = current;
        current = next;
    }

    return 0;
}



int List_RemoveAt(List* list, int index) {
    if (!list || index < 0 || index >= list->size) return 0;

    Node* current = list->head;
    Node* previous = NULL;

    for (int i = 0; i < index; i++) {
        previous = current;
        current = current->next;
    }

    if (previous) previous->next = current->next;
    else list->head = current->next;

    if (current == list->tail) list->tail = previous;

    free(current->data);
    free(current);
    list->size--;

    return 1;
}


// Remove o primeiro elemento da lista.
int List_RemoveFirst(List* list) {
    if (!list || !list->head) return 0;

    Node* node_to_remove = list->head;
    list->head = node_to_remove->next;

    if (list->head == NULL) {
        list->tail = NULL;
    }

    free(node_to_remove->data);
    free(node_to_remove);
    list->size--;
    return 1;
}

int List_Destroy(List* target) {
    if (!target) return 0;

    Node* current = target->head;
    while (current) {
        Node* next = current->next;
        free(current->data);
        free(current);
        current = next;
    }

    ListInfo* current_info = lists;
    ListInfo* previous_info = NULL;
    while (current_info) {
        if (current_info->list == target) {
            if (previous_info)
                previous_info->next = current_info->next;
            else
                lists = current_info->next;
            free(current_info);
            break;
        }
        previous_info = current_info;
        current_info = current_info->next;
    }

    free(target);
    return 1;
}

// Libera todas as listas criadas, liberando tamb�m os dados de cada n�.
int List_FreeAll() {
    ListInfo* current_info = lists;
    while (current_info) {
        List* list = current_info->list;

        Node* current_node = list->head;
        while (current_node) {
            Node* next_node = current_node->next;
            free(current_node->data);
            free(current_node);
            current_node = next_node;
        }
        free(list);

        ListInfo* next_info = current_info->next;
        free(current_info);
        current_info = next_info;
    }
    lists = NULL;
    return 1;
}

void* List_GetByIndex(List* list, int index) {
    if (!list || index < 0 || index >= list->size) return NULL;

    Node* current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current ? current->data : NULL;
}


// Daqui pra baixo � a parte funcional.

// Aplica uma fun��o a cada n� da lista.
void List_ForEach(List* list, Function fn) {
    if (!list || !fn) return;

    Node* current = list->head;
    while (current) {
        fn(current->data);
        current = current->next;
    }
}

void List_ForEachCtx(List* list, void* context, TwoArgFunction fn) {
    if (!list || !fn) return;

    Node* current = list->head;
    while (current) {
        Node* next = current->next;
        fn(context, current->data);
        current = next;
    }
}


// Encontra um elemento na lista usando um predicado (match function).
void* List_FindWithFn(List* list, void* context, MatchFunction matchfn) {
    if (!list || !matchfn) return NULL;

    Node* current = list->head;
    while (current) {
        if (matchfn(context, current->data)) {
            return current->data;
        }
        current = current->next;
    }
    return NULL;
}


// Adiciona um elemento na lista em ordem, usando uma fun��o de compara��o.
int List_AddWithFn(List* list, void* data, ComparisonFunc comparefn) {
    if (!list || !data || !comparefn) return 0;

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return 0;

    new_node->data = malloc(list->data_size);

    if (!new_node->data) {
        free(new_node);
        return 0;
    }

    memcpy(new_node->data, data, list->data_size);

    if (list->head == NULL || comparefn(new_node->data, list->head->data) < 0) {
        new_node->next = list->head;

        list->head = new_node;

        if (list->tail == NULL) list->tail = new_node;

        list->size++;

        return 1;
    }

    Node* current = list->head;

    while (current->next != NULL && comparefn(new_node->data, current->next->data) >= 0) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;

    if (new_node->next == NULL) list->tail = new_node;

    list->size++;
    return 1;
}

// Remove todos os n�s que a matchfunction pega
int List_RemoveWithFn(List* list, void* context, MatchFunction matchfn) {
    if (!list || !matchfn || list->size == 0) return 0;

    Node* current = list->head;
    Node* previous = NULL;
    int removed_count = 0;

    while (current) {
        Node* next = current->next;

        bool should_remove = matchfn(context, current->data);

        if (should_remove) {
            if (previous) previous->next = next;
            else list->head = next;

            if (current == list->tail) list->tail = previous;

            free(current->data);
            free(current);
            list->size--;
            removed_count++;
        }
        else {
            previous = current;
        }

        current = next;
    }

    return removed_count;
}
