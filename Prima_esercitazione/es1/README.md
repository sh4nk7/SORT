# Esercizio 1

## Obiettivo

Questo esercizio mostra il primo uso di `std::thread` in C++: invece di stampare
`Hello World!` direttamente dal `main`, il programma crea un thread separato che
esegue una funzione dedicata.

## File principali

- `src/main.cpp`: contiene il codice dell'esercizio.
- `Makefile`: contiene i comandi di compilazione.

## Come compilare ed eseguire

Da questa cartella:

```bash
make
./hello_thread
```

Per eliminare l'eseguibile generato:

```bash
make clean
```

## Spiegazione passo passo

1. Il programma include `<thread>` per usare `std::thread`.
2. Include `<chrono>` per usare una pausa temporizzata.
3. La funzione `print_hello()` aspetta un secondo con `std::this_thread::sleep_for`.
4. Dopo l'attesa, `print_hello()` stampa `Hello World!`.
5. Nel `main` viene creato un oggetto `std::thread worker(print_hello)`.
6. La creazione di `worker` fa partire subito un nuovo thread che esegue `print_hello`.
7. Il `main` chiama `worker.join()`.
8. `join()` blocca il `main` finche' il thread non ha terminato.
9. Quando il thread finisce, il programma ritorna `0` e termina correttamente.

## Perche' serve `join`

Un thread deve essere gestito prima che l'oggetto `std::thread` venga distrutto.
Con `join()` il thread principale aspetta la fine del thread secondario. Senza
questa chiamata, il programma terminerebbe in modo anomalo perche' ci sarebbe un
thread ancora "joinable".

## Concetti importanti

- Un thread e' un flusso di esecuzione indipendente.
- `std::thread` riceve una funzione da eseguire.
- `join()` sincronizza il thread principale con quello secondario.
- La stampa avviene nel thread secondario, non direttamente nel `main`.
