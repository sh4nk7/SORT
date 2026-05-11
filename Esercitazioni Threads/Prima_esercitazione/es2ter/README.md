# Esercizio 2ter

## Obiettivo

Questo esercizio completa la versione multithread del prodotto tra matrici usando
blocchi di righe consecutive. A differenza di `es2`, qui non viene creato un
thread per ogni riga: viene scelto un numero limitato di thread e ogni thread
calcola piu' righe.

Questa soluzione e' piu' realistica per matrici grandi, perche' evita di creare
centinaia o migliaia di thread.

## File principali

- `src/main.cpp`: contiene prodotto seriale, prodotto multithread a blocchi e benchmark.
- `Makefile`: compila il programma.
- `../es2/src/matrix.h`: contiene la classe `matrix` riusata anche qui.

## Come compilare ed eseguire

Da questa cartella:

```bash
make
./matrix_block_threads
```

Per scegliere la dimensione della matrice:

```bash
./matrix_block_threads 100
```

Per scegliere dimensione e numero di thread:

```bash
./matrix_block_threads 100 4
```

Per eliminare l'eseguibile:

```bash
make clean
```

## Spiegazione passo passo

1. Il programma legge la dimensione della matrice dal primo argomento.
2. Se la dimensione non viene passata, usa `STD_SIZE`, cioe' 10.
3. Il programma legge il numero di thread dal secondo argomento.
4. Se il numero di thread non viene passato, usa `std::thread::hardware_concurrency()`.
5. Se `hardware_concurrency()` restituisce 0, il programma usa 1 thread.
6. Crea le matrici `MA`, `MB`, `serial_result` e `threaded_result`.
7. Inizializza `MA` con tutti 1.
8. Inizializza `MB` con `i + j + 1`.
9. Calcola prima il prodotto seriale.
10. Calcola poi il prodotto multithread a blocchi.
11. Misura i tempi di entrambe le versioni.
12. Confronta i risultati con `same_matrix`.
13. Stampa dimensione, thread usati, tempi e correttezza del risultato.

## Divisione in blocchi di righe

La funzione centrale e' `threaded_product`.

Prima limita il numero di thread:

```cpp
thread_count = std::max<size_t>(1, std::min(thread_count, size));
```

Questo garantisce che:

- venga usato almeno 1 thread;
- non vengano creati piu' thread delle righe disponibili.

Poi calcola quante righe assegnare a ciascun thread:

```cpp
const size_t rows_per_thread = size / thread_count;
const size_t extra_rows = size % thread_count;
```

`rows_per_thread` contiene il numero minimo di righe per thread. `extra_rows`
contiene le righe avanzate quando la divisione non e' perfetta.

Esempio con 10 righe e 4 thread:

- `rows_per_thread = 10 / 4 = 2`;
- `extra_rows = 10 % 4 = 2`;
- i primi 2 thread ricevono una riga extra.

Quindi i blocchi diventano:

```text
thread 0: righe 0, 1, 2
thread 1: righe 3, 4, 5
thread 2: righe 6, 7
thread 3: righe 8, 9
```

## Funzione `compute_row_block`

Ogni thread esegue:

```cpp
compute_row_block(first_row, last_row, M1, M2, Res)
```

La funzione calcola tutte le righe nell'intervallo:

```cpp
for (size_t row = first_row; row < last_row; ++row)
```

L'intervallo e' semiaperto: include `first_row`, ma esclude `last_row`. Questo
rende piu' semplice dividere i blocchi senza sovrapposizioni.

## Perche' non serve un mutex

Ogni thread riceve un blocco di righe diverso. Quindi due thread non scrivono mai
sulla stessa cella di `threaded_result`. Le matrici di input sono passate come
riferimenti costanti e vengono solo lette.

Per questo motivo non c'e' una race condition sui dati della matrice risultato e
non serve usare `std::mutex`.

## Differenza rispetto a `es2`

In `es2` il numero di thread coincide con il numero di righe. Con una matrice
1000x1000 verrebbero creati 1000 thread.

In `es2ter`, invece, si puo' scegliere un numero piu' ragionevole:

```bash
./matrix_block_threads 1000 8
```

In questo caso vengono creati 8 thread e ciascuno calcola circa 125 righe.

## Controllo del risultato

Il programma calcola anche una versione seriale e poi confronta le due matrici
risultato. Se tutto e' corretto, l'ultima riga stampata e':

```text
Risultati uguali: si
```
