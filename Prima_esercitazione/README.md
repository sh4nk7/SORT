# Esercitazione Real-Time 1

Questa cartella contiene la prima esercitazione sui thread in C++11. Gli esercizi introducono la creazione di thread con `std::thread`, la sincronizzazione con `join()`, il calcolo parallelo del prodotto tra matrici e la misura dei tempi di esecuzione con la libreria `<chrono>`.

Il file `es1.pdf` contiene la traccia originale dell'esercitazione. Le cartelle `es1`, `es2`, `es2bis` ed `es2ter` contengono invece le soluzioni organizzate per esercizio, ognuna con sorgenti, `Makefile` e README dedicato.

## Struttura della cartella

```text
Prima_esercitazione/
├── README.md
├── es1.pdf
├── es1/       primo uso di std::thread
├── es2/       prodotto tra matrici con un thread per riga
├── es2bis/    benchmark seriale vs multithread
└── es2ter/    prodotto a blocchi con numero controllato di thread
```

Ogni sottocartella contiene:

- `src/main.cpp`, cioe il sorgente principale dell'esercizio;
- `Makefile`, per compilare e pulire l'eseguibile;
- `README.md`, con spiegazione dettagliata del singolo esercizio;
- eventuali file di supporto, come `src/matrix.h` per gli esercizi sulle matrici.

## Compilazione generale

Per compilare un esercizio, entrare nella sua cartella e lanciare `make`.

Esempio:

```sh
cd es1
make
./hello_thread
```

Per eliminare l'eseguibile generato:

```sh
make clean
```

Tutti gli esercizi sono compilati con `g++`, standard C++11 e supporto ai thread POSIX tramite `-pthread`.

## Esercizio 1

Cartella:

```text
es1/
```

Eseguibile:

```text
hello_thread
```

Obiettivo:

L'esercizio parte da un programma sequenziale che stampa `Hello World!` dopo una pausa di un secondo. La richiesta e trasformarlo in un programma multithread: il `main` deve creare un thread figlio che esegue la funzione `print_hello()`, poi deve attendere la conclusione del thread tramite `join()`.

Concetti principali:

- creazione di un thread con `std::thread`;
- passaggio di una funzione al costruttore del thread;
- uso di `std::this_thread::sleep_for`;
- sincronizzazione del thread principale tramite `join()`;
- differenza tra eseguire una funzione direttamente ed eseguirla in un thread separato.

Comandi:

```sh
cd es1
make
./hello_thread
```

## Esercizio 2

Cartella:

```text
es2/
```

Eseguibile:

```text
matrix_threads
```

Obiettivo:

L'esercizio implementa il prodotto tra due matrici quadrate usando piu thread concorrenti. La strategia adottata e creare un thread per ogni riga della matrice risultato: ogni thread calcola tutte le colonne della propria riga.

La formula del prodotto e:

```text
C[i][j] = somma di A[i][k] * B[k][j]
```

per tutti i valori di `k` da `0` a `N - 1`.

Concetti principali:

- decomposizione del lavoro per righe;
- uso di un vettore di `std::thread`;
- passaggio di parametri ai thread;
- uso di `std::ref()` e `std::cref()` per passare riferimenti;
- assenza di mutex quando ogni thread scrive in una riga diversa;
- riuso della classe `matrix` definita in `src/matrix.h`.

Comandi:

```sh
cd es2
make
./matrix_threads
```

Per scegliere la dimensione della matrice:

```sh
./matrix_threads 100
```

## Esercizio 2bis

Cartella:

```text
es2bis/
```

Eseguibile:

```text
matrix_benchmark
```

Obiettivo:

Questo esercizio estende il prodotto tra matrici confrontando due versioni:

- una versione seriale, eseguita interamente dal thread principale;
- una versione multithread, con un thread per ogni riga della matrice risultato.

Lo scopo non e soltanto calcolare il prodotto, ma misurare i tempi delle due strategie e verificare che producano lo stesso risultato.

Concetti principali:

- misura del tempo con `std::chrono`;
- confronto tra algoritmo seriale e algoritmo multithread;
- valutazione del costo di creazione e sincronizzazione dei thread;
- controllo di correttezza confrontando le matrici risultato;
- osservazione dello speed-up al variare della dimensione dei dati.

Comandi:

```sh
cd es2bis
make
./matrix_benchmark
```

Per scegliere la dimensione della matrice:

```sh
./matrix_benchmark 100
```

Output atteso:

Il programma stampa informazioni come dimensione della matrice, tempo della versione seriale, tempo della versione multithread e risultato del confronto tra le due matrici.

## Esercizio 2ter

Cartella:

```text
es2ter/
```

Eseguibile:

```text
matrix_block_threads
```

Obiettivo:

Questo esercizio migliora la versione multithread dell'esercizio 2. Invece di creare un thread per ogni riga, crea un numero controllato di thread e assegna a ciascuno un blocco di righe consecutive.

Questa soluzione e piu realistica per matrici grandi, perche evita di creare un numero eccessivo di thread.

Concetti principali:

- uso di `std::thread::hardware_concurrency()`;
- scelta del numero di thread;
- divisione delle righe in blocchi;
- gestione di divisioni non perfette tramite righe extra;
- confronto delle prestazioni rispetto alla versione seriale;
- verifica della correttezza del risultato.

Comandi:

```sh
cd es2ter
make
./matrix_block_threads
```

Per scegliere la dimensione della matrice:

```sh
./matrix_block_threads 100
```

Per scegliere sia dimensione sia numero di thread:

```sh
./matrix_block_threads 100 4
```

## Differenza tra gli esercizi sulle matrici

`es2` serve a capire il parallelismo di base: ogni thread calcola una singola riga.

`es2bis` aggiunge il confronto prestazionale tra versione seriale e versione multithread, usando misure temporali con `<chrono>`.

`es2ter` rende la soluzione piu scalabile: invece di creare un thread per riga, usa un numero limitato di thread e assegna a ciascuno piu righe.

In breve:

```text
es2     -> N righe, N thread
es2bis  -> seriale vs N thread, con benchmark
es2ter  -> seriale vs blocchi di righe, con numero di thread controllato
```

## Note su `std::chrono`

Per misurare i tempi, gli esercizi usano la libreria standard `<chrono>`.

Il flusso tipico e:

```cpp
const auto start = std::chrono::high_resolution_clock::now();

// codice da misurare

const auto end = std::chrono::high_resolution_clock::now();
std::chrono::duration<double, std::milli> elapsed(end - start);
```

Il valore `elapsed.count()` restituisce il tempo trascorso in millisecondi.

## Note su `std::thread`

Un oggetto `std::thread` rappresenta un flusso di esecuzione separato. Quando viene creato, il thread parte subito.

Esempio:

```cpp
std::thread worker(print_hello);
worker.join();
```

La chiamata a `join()` e fondamentale: il thread principale resta in attesa finche il thread figlio non termina. Se un oggetto `std::thread` viene distrutto mentre e ancora `joinable`, il programma termina in modo anomalo.

## Verifica rapida

Per compilare tutti gli esercizi manualmente:

```sh
cd es1
make
cd ../es2
make
cd ../es2bis
make
cd ../es2ter
make
```

Per pulire ogni esercizio:

```sh
cd es1
make clean
cd ../es2
make clean
cd ../es2bis
make clean
cd ../es2ter
make clean
```

Gli eseguibili prodotti sono:

- `es1/hello_thread`;
- `es2/matrix_threads`;
- `es2bis/matrix_benchmark`;
- `es2ter/matrix_block_threads`.
