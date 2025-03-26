# 🏛️ Visita Guidata (Server-Client)

Questo progetto implementa un sistema distribuito per la gestione di visite guidate a un monumento, simulando un’interazione tra **guide turistiche**, **gruppi di visitatori** e un **server centrale** di coordinamento. È sviluppato in linguaggio **C** utilizzando **socket TCP** in ambiente **Unix/Linux**.

---

## 📚 Descrizione del Sistema

Il sistema prevede tre tipi di processi:

- 🧭 **Server (`server.c`)**: coordina l'incontro tra guide e gruppi visitatori.
- 👨‍🏫 **Guida (`guida.c`)**: si registra al server indicando nome, minimo e massimo visitatori.
- 👥 **Visitatori (`visitatori.c`)**: si registra specificando il numero di componenti del gruppo.

Il server gestisce la disponibilità delle guide e organizza una visita quando una guida ha raggiunto il numero minimo di partecipanti.

---

## 📂 Struttura del Progetto

```
visita-guidata/
├── src/                 # Codice sorgente
│   ├── server.c         # Processo server
│   ├── guida.c          # Processo guida turistica
│   ├── visitatori.c     # Processo gruppo di visitatori
│   └── utils.c          # Funzioni di utilità
├── include/             # Header files
│   ├── common.h         # Strutture condivise
│   └── utils.h          # Interfaccia funzioni di utilità
├── bin/                 # File eseguibili generati
├── docs/                # Documentazione
│   └── README.md
├── Makefile             # File di compilazione
```

---

## ⚙️ Compilazione

Assicurati di avere `gcc` installato, poi esegui:

```bash
make
```

Verranno creati gli eseguibili all'interno della cartella `bin/`.

---

## 🧪 Esecuzione

Apri **almeno 3 terminali**.

### 1️⃣ Server (Terminale 1)
```bash
./bin/server
```

### 2️⃣ Guida (Terminale 2)
```bash
./bin/guida Mario 3 10
```
Registra la guida “Mario” con minimo 3 e massimo 10 visitatori.

### 3️⃣ Visitatori (Terminale 3+)
```bash
./bin/visitatori 2
```
Registra un gruppo di 2 persone. Se viene raggiunto il numero minimo della guida, la visita inizia.

---

## 🧼 Pulizia

Per eliminare tutti i file binari:
```bash
make clean
```

---

## 🧵 Dettagli Tecnici

- Comunicazione tramite **socket TCP** (`AF_INET`, `SOCK_STREAM`)
- Uso di **struct condivise** (`common.h`) per messaggi tra processi
- Gestione concorrente con **pthreads** nel server
- Ogni guida può accumulare gruppi finché non raggiunge la soglia
- Il server seleziona la guida più adatta con minimo “spreco” di posti

---

## 💡 Possibili Estensioni

- Assegnazione prioritaria a gruppi più grandi
- Timeout per guide inattive
- Supporto a più visite simultanee
- Logging su file o interfaccia grafica

---

## 👤 Autore

**Giuseppe Dimonte** 
📧 giuseppe.dimonte@studenti.unipr.it 
🎓 Università degli Studi di Parma

---

