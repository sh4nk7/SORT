# Esercitazione Real-Time 2

Questa cartella contiene la suddivisione degli esercizi dell'esercitazione Real-Time n.2 sui thread POSIX, sulle priorita real-time, sull'affinita dei thread e sulla temporizzazione periodica in C++.

La cartella `es2-code` contiene i file originali forniti dal professore ed e stata lasciata invariata. Le cartelle `esercizio_*` sono invece copie separate del materiale di partenza, organizzate in modo da lavorare su un esercizio alla volta senza mischiare le soluzioni.

## Struttura della cartella

```text
Seconda_esercitazione/
├── README.md
├── es2-code/          materiale originale del professore
├── esercizio_1/       priorita real-time su hello.cpp
├── esercizio_1bis/    priorita real-time + affinita su hello-rt.cpp
├── esercizio_2/       temporizzazione con sleep_until
└── esercizio_2bis/    temporizzazione Fibonacci con sleep_until
```

Ogni cartella di esercizio contiene:

- i sorgenti necessari per quell'esercizio;
- la sottocartella `rt`, con la libreria wrapper `librt_pthread`;
- una `Makefile` locale;
- eventuali file di supporto, come `barrier.cpp` e `barrier.h`.

## Compilazione generale

Per compilare un esercizio, entrare nella sua cartella ed eseguire `make`.

Esempio:

```sh
cd esercizio_1
make
```

Per eliminare file oggetto, librerie compilate ed eseguibili:

```sh
make clean
```

La compilazione usa `g++`, C++11 e il supporto ai thread POSIX tramite `-pthread`.

## Esecuzione e privilegi

Gli esercizi che impostano priorita real-time possono richiedere privilegi elevati. Se il programma stampa un errore relativo ai permessi quando prova a impostare lo scheduling real-time, eseguirlo con `sudo`.

Esempio:

```sh
sudo ./hello
```

oppure, per l'esercizio `1bis`:

```sh
sudo ./hello-rt
```

La necessita di `sudo` dipende dalla configurazione del sistema operativo e dai permessi concessi all'utente corrente.

## Esercizio 1

Cartella:

```text
esercizio_1/
```

File principali:

- `hello.cpp`
- `barrier.cpp`
- `barrier.h`
- `rt/priority.h`

Obiettivo:

Il programma crea un thread per ogni carattere della stringa `Hello World!`. Ogni thread attende su una barriera e, quando tutti i thread sono pronti, stampa il proprio carattere.

Nella versione iniziale l'ordine di stampa non e deterministico, perche dipende dall'ordine con cui i thread vengono risvegliati dalla barriera.

La modifica richiesta consiste nell'assegnare ai thread figli opportune priorita real-time, in modo che su una macchina uniprocessore l'ordine di esecuzione diventi deterministico e la stringa stampata sia:

```text
Hello World!
```

Vincoli importanti:

- non bisogna introdurre nuove sincronizzazioni esplicite tra i thread;
- bisogna agire tramite politica di scheduling e priorita;
- il comportamento richiesto e riferito al caso uniprocessore.

Comandi:

```sh
cd esercizio_1
make
sudo ./hello
```

## Esercizio 1bis

Cartella:

```text
esercizio_1bis/
```

File principali:

- `hello-rt.cpp`
- `barrier.cpp`
- `barrier.h`
- `rt/priority.h`
- `rt/affinity.h`

Obiettivo:

Questo esercizio estende il precedente al caso multiprocessore o multicore.

Su una macchina con piu core, impostare soltanto le priorita real-time non basta necessariamente a garantire l'ordine corretto di stampa, perche piu thread potrebbero essere eseguiti contemporaneamente su core diversi.

La modifica richiesta consiste quindi nel:

- mantenere l'uso delle priorita real-time;
- impostare l'affinita dei thread figli;
- fare in modo che tutti i thread che stampano la stringa vengano eseguiti sullo stesso core.

In questo modo si recupera un comportamento equivalente a quello di una macchina uniprocessore, rendendo di nuovo deterministico l'ordine di esecuzione.

Comandi:

```sh
cd esercizio_1bis
make
sudo ./hello-rt
```

## Esercizio 2

Cartella:

```text
esercizio_2/
```

File principali:

- `timing.cpp`
- `rt/priority.h`

Obiettivo:

Il programma esegue periodicamente la funzione `do_some_stuff()`. Nella versione iniziale, dopo ogni esecuzione della funzione viene usata una pausa relativa con `sleep_for`.

Questo approccio introduce deriva temporale, perche il periodo effettivo diventa:

```text
tempo di esecuzione di do_some_stuff() + tempo di sleep_for()
```

La modifica richiesta consiste nell'usare pause assolute tramite:

```cpp
std::this_thread::sleep_until(...)
```

L'idea e calcolare gli istanti assoluti di attivazione rispetto a un clock monotono, preferibilmente `std::chrono::steady_clock`, cosi la temporizzazione non dipende dalla durata variabile di `do_some_stuff()`.

Comandi:

```sh
cd esercizio_2
make
sudo ./timing
```

## Esercizio 2bis

Cartella:

```text
esercizio_2bis/
```

File principali:

- `timing.cpp`
- `rt/priority.h`

Obiettivo:

Questo esercizio estende l'esercizio 2. La funzione `do_some_stuff()` deve essere eseguita 8 volte seguendo la successione di Fibonacci come sequenza di temporizzazione:

```text
1s 1s 2s 3s 5s 8s 13s 21s
```

Anche in questo caso la temporizzazione deve essere basata su istanti assoluti e non su pause relative, in modo da non accumulare deriva dovuta al tempo di esecuzione della funzione.

Una possibile strategia e:

- salvare un istante iniziale con `std::chrono::steady_clock::now()`;
- mantenere un tempo assoluto della prossima attivazione;
- a ogni iterazione sommare il prossimo intervallo Fibonacci;
- usare `std::this_thread::sleep_until()` per attendere quell'istante.

Comandi:

```sh
cd esercizio_2bis
make
sudo ./timing
```

## Note sui file originali

La cartella `es2-code` non va modificata se si vuole conservare una copia pulita del materiale del professore.

I file di partenza sono:

- `hello.cpp`, usato per l'esercizio 1;
- `hello-rt.cpp`, usato per l'esercizio 1bis;
- `timing.cpp`, usato per gli esercizi 2 e 2bis;
- `barrier.cpp` e `barrier.h`, usati dagli esercizi sulla stampa di `Hello World!`;
- `rt/`, libreria wrapper per priorita real-time e affinita.

## Verifica rapida

Per controllare che una cartella sia compilabile:

```sh
make
```

Per controllare che l'eseguibile funzioni:

```sh
sudo ./nome_eseguibile
```

Dove `nome_eseguibile` e:

- `hello` per `esercizio_1`;
- `hello-rt` per `esercizio_1bis`;
- `timing` per `esercizio_2` ed `esercizio_2bis`.
