#include "list.h"

/* Inserisce un elemento in prima posizione */
LIST EnqueueFirst(LIST l, itemType item) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("Errore di allocazione memoria");
        exit(EXIT_FAILURE);
    }
    newNode->data = item;
    newNode->next = l;
    return newNode;  // Nuova testa della lista
}

/* Rimuove l'ultimo elemento dalla lista */
LIST DequeueLast(LIST l) {
    if (l == NULL) return NULL;  // Lista vuota

    if (l->next == NULL) {  // Unico elemento
        free(l);
        return NULL;
    }

    Node *prev = NULL, *curr = l;
    while (curr->next != NULL) {
        prev = curr;
        curr = curr->next;
    }

    free(curr);
    prev->next = NULL;
    return l;
}

/* Inserisce un elemento mantenendo l'ordine crescente */
LIST EnqueueOrdered(LIST l, itemType item) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("Errore di allocazione memoria");
        exit(EXIT_FAILURE);
    }
    newNode->data = item;
    newNode->next = NULL;

    if (l == NULL || item < l->data) {  // Inserimento in testa
        newNode->next = l;
        return newNode;
    }

    Node* curr = l;
    while (curr->next != NULL && curr->next->data < item) {
        curr = curr->next;
    }

    newNode->next = curr->next;
    curr->next = newNode;

    return l;
}

/* Trova un elemento nella lista e restituisce un puntatore al dato */
itemType* Find(LIST l, itemType item) {
    while (l != NULL) {
        if (l->data == item) {
            return &(l->data);  // Restituisce puntatore ai dati, non al nodo
        }
        l = l->next;
    }
    return NULL;  // Elemento non trovato
}

/* Stampa la lista */
void PrintList(LIST l) {
    while (l != NULL) {
        printf("%d -> ", l->data);
        l = l->next;
    }
    printf("NULL\n");
}

/* Libera la memoria della lista */
void FreeList(LIST l) {
    Node* temp;
    while (l != NULL) {
        temp = l;
        l = l->next;
        free(temp);
    }
}
