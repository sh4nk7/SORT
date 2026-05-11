# Esercizio 2bis

## Obiettivo

Questo esercizio confronta due versioni del prodotto tra matrici:

- una versione seriale, eseguita interamente dal thread principale;
- una versione multithread, con un thread per ogni riga della matrice risultato.

Lo scopo non e' solo calcolare il prodotto, ma anche misurare i tempi delle due
strategie e verificare che producano lo stesso risultato.

## File principali

- `src/main.cpp`: contiene versione seriale, versione multithread e benchmark.
- `Makefile`: compila il programma.
- `../es2/src/matrix.h`: contiene la classe `matrix` riusata da questo esercizio.

## Come compilare ed eseguire

Da questa cartella:

```bash
make
./matrix_benchmark
```

Per scegliere la dimensione della matrice:

```bash
./matrix_benchmark 100
```

Per eliminare l'eseguibile:

```bash
make clean
```

## Spiegazione passo passo

1. Il programma legge la dimensione della matrice da riga di comando.
2. Se non viene passato nessun parametro, usa `STD_SIZE`, cioe' 10.
3. Crea quattro matrici: `MA`, `MB`, `serial_result` e `threaded_result`.
4. `fill_matrices` inizializza le due matrici di input.
5. `MA` viene riempita con tutti 1.
6. `MB` viene riempita con `i + j + 1`.
7. Se la dimensione e' minore di `MAX_PRINT_SIZE`, stampa le matrici iniziali.
8. Misura il tempo della versione seriale usando `std::chrono`.
9. Misura il tempo della versione multithread usando ancora `std::chrono`.
10. Se la matrice e' piccola, stampa entrambi i risultati.
11. Confronta le due matrici risultato con `same_matrix`.
12. Stampa dimensione, tempi e risultato del confronto.

## Versione seriale

La funzione `serial_product` attraversa tutte le righe:

```cpp
for (size_t row = 0; row < Res.size(); ++row)
```

Per ogni riga richiama `compute_row`, che calcola tutte le colonne della riga.
Tutto avviene nel thread principale, quindi non c'e' lavoro parallelo.

## Versione multithread

La funzione `threaded_product` crea un vettore di thread:

```cpp
std::vector<std::thread> workers;
```

Poi crea un thread per ogni riga:

```cpp
workers.emplace_back(compute_row, row, std::cref(M1), std::cref(M2), std::ref(Res));
```

Qui sono importanti:

- `std::cref(M1)` e `std::cref(M2)`: passano le matrici di input come riferimenti costanti;
- `std::ref(Res)`: passa la matrice risultato per riferimento, cosi' ogni thread puo' scriverci dentro.

Dopo la creazione dei thread, il programma chiama `join()` su ciascuno per
aspettare che tutti finiscano.

## Misura dei tempi

Il tempo viene misurato prendendo due istanti:

```cpp
const auto start = std::chrono::high_resolution_clock::now();
const auto end = std::chrono::high_resolution_clock::now();
```

La differenza tra `end` e `start` viene convertita in millisecondi:

```cpp
std::chrono::duration<double, std::milli>
```

## Perche' si confrontano i risultati

Il multithreading puo' introdurre errori difficili da vedere, soprattutto quando
piu' thread scrivono su dati condivisi. In questo programma ogni thread scrive
una riga diversa, quindi la strategia e' corretta, ma `same_matrix` serve come
controllo pratico: se il risultato seriale e quello multithread sono uguali, il
programma stampa `si`.

## Nota sulle prestazioni

La versione multithread non e' sempre piu' veloce. Creare thread ha un costo. Su
matrici piccole il tempo speso per creare e sincronizzare i thread puo' essere
maggiore del vantaggio ottenuto. Il beneficio diventa piu' visibile con matrici
abbastanza grandi.
