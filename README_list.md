Libreria per Liste Concatenate in C

Descrizione

Questa libreria implementa una lista concatenata con funzioni per l'inserimento, la rimozione e la ricerca di elementi.

Struttura del Progetto

Il progetto è suddiviso in tre file per garantire una buona organizzazione del codice:

list.h → Dichiarazioni della struttura dati e delle funzioni.

list.c → Implementazione delle funzioni.

main.c → Esempio di utilizzo della libreria.

Funzionalità

EnqueueFirst(LIST l, itemType item) → Inserisce un elemento in prima posizione.

DequeueLast(LIST l) → Rimuove l'ultimo elemento dalla lista.

EnqueueOrdered(LIST l, itemType item) → Inserisce un elemento mantenendo l'ordine crescente.

Find(LIST l, itemType item) → Trova un elemento e restituisce un puntatore ai suoi dati.

PrintList(LIST l) → Stampa la lista.

FreeList(LIST l) → Libera la memoria della lista.

Compilazione ed Esecuzione

Per compilare il progetto, eseguire:

gcc -o main main.c list.c

Per eseguire il programma:

./main

Output Atteso

Esempio di output del programma:

Aggiunta di elementi all'inizio:
30 -> 20 -> 10 -> NULL
Aggiunta ordinata di elementi:
5 -> 10 -> 20 -> 25 -> 30 -> 35 -> NULL
Rimozione dell'ultimo elemento:
5 -> 10 -> 20 -> 25 -> 30 -> NULL
Ricerca di un elemento:
Elemento 25 trovato nella lista.

Miglioramenti Futuri

Implementare una versione doppia della lista concatenata.

Aggiungere funzioni per la rimozione di un elemento specifico.

Ottimizzare la ricerca con strutture più efficienti.

Autore

Realizzato da Giuseppe Dimonte.
