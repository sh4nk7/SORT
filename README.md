# Attuatori e Sensori (Server-Client)

Questo progetto implementa un sistema distribuito per la gestione del riscaldamento in un edificio utilizzando socket TCP in ambiente Unix/C. Il sistema è composto da:

- Un server (**server.c**) che gestisce la registrazione degli attuatori e la distribuzione delle misure.
- Un hub (**hub.c**) che raccoglie le misure dei sensori e le inoltra al server.
- Client sensori (**sensore.c**) che inviano misure di temperatura casuali.
- Client attuatori (**attuatore.c**) che si iscrivono a specifici sensori e decidono se accendere o spegnere il riscaldamento.

## 📂 Struttura del Progetto
```
attuatori-sensori/
│── 📂 src/              # Codice sorgente
│   │── server.c         # Implementazione del server
│   │── hub.c            # Implementazione del processo hub
│   │── sensore.c        # Implementazione del client sensore
│   │── attuatore.c      # Implementazione del client attuatore
│   │── utils.c          # Funzioni di utilità comuni
│── 📂 include/          # Header files
│   │── common.h         # Costanti e strutture condivise
│   │── utils.h          # Dichiarazioni funzioni utilità
│── 📂 bin/              # Eseguibili compilati
│   │── server           # Eseguibile del server
│   │── hub              # Eseguibile dell'hub
│   │── sensore          # Eseguibile del sensore
│   │── attuatore        # Eseguibile dell'attuatore
│── README.md            # Questo file
│── Makefile             # Per la compilazione automatica
```

## 🚀 Compilazione ed Esecuzione

### 🔧 Compilare il progetto
Con Makefile:
```bash
make
```
Oppure manualmente:
```bash
mkdir -p bin
# Compila tutti i componenti
gcc -Wall -pthread src/server.c src/utils.c -Iinclude -o bin/server
gcc -Wall src/hub.c src/utils.c -Iinclude -o bin/hub
gcc -Wall src/sensore.c src/utils.c -Iinclude -o bin/sensore
gcc -Wall src/attuatore.c src/utils.c -Iinclude -o bin/attuatore
```

### 💻 Eseguire il server
```bash
./bin/server
```
Il server accetta connessioni dagli attuatori e riceve misure dall'hub.

### 💪 Eseguire il processo hub
```bash
./bin/hub 3
```
Aspetta 3 sensori prima di inoltrare le misure.

### 🚀 Eseguire un sensore
```bash
./bin/sensore sensor1 5
```
Avvia 5 figli sensori con identificativo "sensor1" che inviano misure all'hub.

### 🔌 Eseguire un attuatore
```bash
./bin/attuatore A1 20 sensor1 sensor2
```
Si iscrive con nome A1, Tgoal = 20, e sottoscrive ai sensori "sensor1" e "sensor2".

### ❌ Disiscrivere un attuatore
```bash
./bin/attuatore A1
```
Comunica la disiscrizione al server per l'attuatore A1.

### 🔄 Pulizia dei file compilati
Con Makefile:
```bash
make clean
```
Oppure manualmente:
```bash
rm -rf bin/*
```

## 📍 Autore
**Giuseppe Dimonte**

Email: giuseppe.dimonte@studenti.unipr.it  
Università degli Studi di Parma
