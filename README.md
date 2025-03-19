# Borsa Valori (Server-Client)

Questo progetto implementa un sistema di gestione della borsa valori utilizzando socket TCP in ambiente Unix/C. Il sistema è composto da:

- **Un server (`server.c`)** che gestisce le quotazioni dei titoli e le operazioni di acquisto/vendita.
- **Client agenti (`agente.c`)** che registrano i titoli nel mercato.
- **Client investitori (`investitore.c`)** che acquistano i titoli quotati.

## 📂 Struttura del Progetto
```
borsa-valori/
│── 📂 src/            # Codice sorgente
│   │── server.c       # Implementazione del server
│   │── agente.c       # Implementazione del client agente
│   │── investitore.c  # Implementazione del client investitore
│── 📂 include/        # Header files opzionali
│── 📂 bin/            # Eseguibili compilati
│   │── server        # File eseguibile del server
│   │── agente        # File eseguibile del client agente
│   │── investitore   # File eseguibile del client investitore
│── 📂 docs/           # Documentazione opzionale
│   │── README.md     # Questo file
│── Makefile          # File per la compilazione automatica
```

## 🚀 Compilazione ed Esecuzione
### 🔧 Compilare il progetto
Se hai un Makefile, puoi semplicemente eseguire:
```sh
make
```
Oppure, per compilare manualmente:
```sh
mkdir -p bin
# Compila il server
gcc -Wall -pthread src/server.c -o bin/server
# Compila il client agente
gcc -Wall src/agente.c -o bin/agente
# Compila il client investitore
gcc -Wall src/investitore.c -o bin/investitore
```

### 🖥️ Eseguire il server
Dopo la compilazione, avvia il server:
```sh
./bin/server
```
Il server attende connessioni da agenti e investitori.

### 🎮 Eseguire un client agente
Apri un nuovo terminale ed esegui:
```sh
./bin/agente Pippo 10 15 12
```
Registra un titolo con il nome "Pippo", 10 azioni, prezzo unitario 15 e prezzo minimo 12.

### 🏆 Eseguire un client investitore
Apri un altro terminale ed esegui:
```sh
./bin/investitore
```
Il client mostrerà i titoli disponibili e permetterà di acquistarne uno.

### 🔄 Pulizia dei file compilati
Per eliminare i file compilati ed eseguibili:
```sh
make clean
```
Oppure manualmente:
```sh
rm -rf bin/*
```

## 📝 Autore
**Giuseppe Dimonte**
- Email: giuseppe.dimonte@studenti.unipr.it
- Università degli Studi di Parma
