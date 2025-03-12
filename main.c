#include "list.h"

int main() {
    LIST myList = NULL;

    printf("Aggiunta di elementi all'inizio:\n");
    myList = EnqueueFirst(myList, 10);
    myList = EnqueueFirst(myList, 20);
    myList = EnqueueFirst(myList, 30);
    PrintList(myList);

    printf("Aggiunta ordinata di elementi:\n");
    myList = EnqueueOrdered(myList, 25);
    myList = EnqueueOrdered(myList, 5);
    myList = EnqueueOrdered(myList, 35);
    PrintList(myList);

    printf("Rimozione dell'ultimo elemento:\n");
    myList = DequeueLast(myList);
    PrintList(myList);

    printf("Ricerca di un elemento:\n");
    itemType *found = Find(myList, 25);
    if (found) {
        printf("Elemento %d trovato nella lista.\n", *found);
    } else {
        printf("Elemento non trovato.\n");
    }

    FreeList(myList);
    return 0;
}
