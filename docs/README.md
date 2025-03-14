Gioco Multi-Utente

Descrizione

Questo progetto implementa un sistema di gioco multi-utente in ambiente Unix/C utilizzando socket di tipo Stream. Il sistema prevede:

Un server che gestisce i giocatori e le partite.

Client giocatori che si connettono al server per partecipare al gioco.

Il server mantiene un elenco di giocatori con il loro punteggio e numero di partite giocate. Quando il numero di giocatori connessi raggiunge N (>=3), viene simulata una partita e i punteggi vengono aggiornati.

Funzionamento

Il server attende connessioni sulla porta 8080.

I giocatori si connettono al server e forniscono il loro nome.

Se un giocatore è nuovo, viene creato il suo profilo.

Quando si raggiunge il numero minimo di giocatori, il server simula la partita:

Assegna i punteggi ai primi tre classificati in modo casuale.

Aggiorna il database dei giocatori.

Invia i risultati ai client.

I giocatori ricevono l'esito della partita e terminano.

Il server attende nuovi giocatori per la prossima partita.

Compilazione e Avvio

Server

Per compilare ed eseguire il server:

gcc server.c -o server -pthread
./server

Client

Per compilare ed eseguire un client giocatore:

gcc client.c -o client
./client

Struttura dei File

server.c: Codice del server.

client.c: Codice del client.

README.md: Questo file.

Esempio di Output

Server:

Server in ascolto sulla porta 8080...
Giocatore connesso: Alice (0 partite, 0 punti)
Giocatore connesso: Bob (0 partite, 0 punti)
Giocatore connesso: Charlie (0 partite, 0 punti)
Simulazione partita...
Classifica: 1° Alice, 2° Bob, 3° Charlie

Client:

Inserisci il tuo nome: Alice
Risultato partita: Classifica: 1° Alice, 2° Bob, 3° Charlie

Autore

Progetto sviluppato in linguaggio C per l'interazione tra processi tramite socket TCP.
