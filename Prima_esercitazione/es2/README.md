# Esercizio 2

## Obiettivo

L'esercizio calcola il prodotto tra due matrici quadrate usando piu' thread. La
strategia scelta e' semplice: ogni thread calcola una riga completa della matrice
risultato.

La base e' presa dai file di esempio in `es1-code/matrix`.

## File principali

- `src/main.cpp`: contiene la logica del prodotto multithread.
- `src/matrix.h`: definisce la classe `matrix`.
- `Makefile`: compila il programma.

## Come compilare ed eseguire

Da questa cartella:

```bash
make
./matrix_threads
```

Per scegliere la dimensione della matrice:

```bash
./matrix_threads 100
```

Per eliminare l'eseguibile:

```bash
make clean
```

## Struttura della matrice

La classe `matrix` usa un `std::vector<int>` monodimensionale per salvare gli
elementi. Anche se la matrice e' concettualmente bidimensionale, internamente gli
elementi sono memorizzati in una sequenza lineare.

L'accesso avviene con:

```cpp
M(i, j)
```

Dentro `operator()`, l'indice bidimensionale viene trasformato in un indice del
vettore:

```cpp
i * dim + j
```

Quindi, se `dim` vale 10, l'elemento alla riga 2 e colonna 3 si trova nella
posizione `2 * 10 + 3`.

## Spiegazione passo passo

1. Il programma legge la dimensione della matrice da riga di comando.
2. Se non viene passato nessun valore, usa `STD_SIZE`, cioe' 10.
3. Crea tre matrici: `MA`, `MB` e `MC`.
4. Riempie `MA` con tutti valori uguali a 1.
5. Riempie `MB` con il valore `i + j + 1`, dove `i` e' la riga e `j` la colonna.
6. Se la matrice e' piccola, stampa `MA` e `MB`.
7. Crea un vettore di thread chiamato `workers`.
8. Per ogni riga della matrice crea un thread.
9. Ogni thread chiama `compute_row` su una riga diversa.
10. `compute_row` calcola tutte le colonne di quella riga.
11. Per ogni elemento richiama `mult`.
12. `mult` applica la formula del prodotto riga per colonna.
13. Dopo aver creato tutti i thread, il `main` richiama `join()` su ciascuno.
14. Quando tutti i thread sono finiti, la matrice `MC` contiene il risultato.
15. Se la matrice e' piccola, il programma stampa anche `MC`.

## Formula del prodotto

Per calcolare l'elemento `MC(row, column)`, il codice somma:

```cpp
MA(row, position) * MB(position, column)
```

per tutte le posizioni da `0` a `size - 1`.

In forma matematica:

```text
MC[row][column] = somma di MA[row][k] * MB[k][column]
```

## Perche' non serve un mutex

Ogni thread scrive una riga diversa di `MC`. Due thread diversi non scrivono mai
nello stesso elemento della matrice risultato. Per questo motivo non serve
proteggere la scrittura con un `mutex`.

Le matrici `MA` e `MB` sono passate come `const matrix &`, quindi vengono solo
lette.

## Attenzione

Questa versione crea un thread per ogni riga. Per matrici piccole va bene come
esercizio didattico, ma per matrici molto grandi puo' creare troppi thread. La
versione `es2ter` risolve questo limite assegnando blocchi di righe a un numero
controllato di thread.
