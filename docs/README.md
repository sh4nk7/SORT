# Gioco Multi-Utente (Server-Client)

Questo progetto implementa un **gioco multi-utente** utilizzando **socket TCP** in ambiente Unix/C. 
Il sistema è composto da:
- Un **server** (`server.c`) che gestisce il gioco e mantiene lo storico dei giocatori.
- Client **giocatori** (`client.c`) che si connettono al server per partecipare alle partite.

---

## 📂 Struttura del Progetto

📁 **gioco-multiutente/** *(cartella principale)*  
│── 📂 **src/** *(codice sorgente)*  
│   │── `server.c` → Implementazione del server  
│   │── `client.c` → Implementazione del client  
│── 📂 **include/** *(header files opzionali)*   
│── 📂 **bin/** *(eseguibili compilati)*  
│   │── `server` → File eseguibile del server  
│   │── `client` → File eseguibile del client  
│── 📂 **docs/** *(documentazione opzionale)*  
│   │── `README.md` → Questo file  
│── `Makefile` → File per la compilazione automatica   

---

## 🚀 Compilazione ed Esecuzione

### 🔧 **Compilare il progetto**
Se hai un `Makefile`, puoi semplicemente eseguire:
```sh
make
```
Oppure, per compilare manualmente:
```sh
mkdir -p bin
# Compila il server
gcc -Wall -pthread src/server.c -o bin/server
# Compila il client
gcc -Wall -pthread src/client.c -o bin/client
```

### 🖥️ **Eseguire il server**
Dopo la compilazione, avvia il server:
```sh
./bin/server
```
Il server attende le connessioni dei client.

### 🎮 **Eseguire un client**
Apri un nuovo terminale ed esegui:
```sh
./bin/client
```
Inserisci il tuo nome quando richiesto e attendi il risultato della partita.

### 🏆 **Simulazione della partita**
- Il server attende **N giocatori** (minimo 3 per iniziare una partita).
- Quando il numero di giocatori è sufficiente, il server **simula una partita casuale**.
- I primi 3 classificati ricevono punti: 🥇 **3 punti**, 🥈 **2 punti**, 🥉 **1 punto**.
- Il risultato viene inviato a tutti i client connessi.

---

## ⚙️ **Funzionamento Interno**

### **Server (`server.c`)**
- Ascolta connessioni in ingresso sulla porta `8080`.
- Mantiene una lista dei giocatori registrati.
- Simula la partita quando ci sono abbastanza giocatori.
- Aggiorna il punteggio dei giocatori e comunica i risultati.

### **Client (`client.c`)**
- Si connette al server.
- Invia il proprio **nome giocatore**.
- Attende il risultato della partita.
- Termina dopo aver ricevuto il risultato.

---

## 🔄 **Pulizia dei file compilati**
Per eliminare i file compilati ed eseguibili:
```sh
make clean
```
Oppure manualmente:
```sh
rm -rf bin/* build/*
```


## 📝 **Autore**
- **Giuseppe DImonte**  
- Email: `giuseppe.dimonte@studenti.unipr.it`  
- Università degli studi di Parma  

---

