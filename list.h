#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int itemType;  // Cambia il tipo dati se necessario

typedef struct Node {
    itemType data;
    struct Node *next;
} Node;

typedef Node* LIST;

/* Funzioni per la gestione della lista */
LIST EnqueueFirst(LIST l, itemType item);
LIST DequeueLast(LIST l);
LIST EnqueueOrdered(LIST l, itemType item);
itemType* Find(LIST l, itemType item);

/* Funzioni di supporto */
void PrintList(LIST l);
void FreeList(LIST l);

#endif
