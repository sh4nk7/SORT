Client-Server Concurrent Requests

Descrizione

Questo progetto implementa un sistema client-server che permette a più client di inviare richieste concorrenti al server. Il server accetta connessioni multiple contemporaneamente e risponde incrementando il numero ricevuto.

Il server è multi-threading e gestisce più client in parallelo.

Il client crea più processi concorrenti che inviano numeri casuali al server.

Struttura del Progetto

server.c → Contiene il codice del server multi-threading.

client.c → Contiene il codice del client concorrente.

Funzionamento

Il server ascolta le connessioni sulla porta 8080.

Quando un client si connette, invia un numero casuale.

Il server riceve il numero, lo incrementa e lo rimanda al client.

Il client stampa il numero inviato e quello ricevuto.

Il processo si ripete per più client simultanei.

Compilazione e Esecuzione

1. Compilare i file

gcc server.c -o server -pthread
gcc client.c -o client

2. Eseguire il server

Avviare il server in un terminale:

./server

3. Eseguire il client

Aprire un altro terminale ed eseguire il client:

./client

Il client genererà più richieste concorrenti al server.

Output Atteso

Esempio di output del server:

Server listening on port 8080...
Received: 45
Received: 23
Received: 67

Esempio di output dei client:

Client 0 sending: 45
Client 0 received: 46
Client 1 sending: 23
Client 1 received: 24
Client 2 sending: 67
Client 2 received: 68

Dettagli Tecnici

Il server usa pthread per gestire più client simultaneamente.

Il client usa fork() per creare processi paralleli.

Per garantire numeri casuali unici, si usa srand(time(NULL) + getpid()).


Autore

Realizzato da Giuseppe Dimonte.
