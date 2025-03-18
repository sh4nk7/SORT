# Libreria Server

## Descrizione
Questo progetto implementa un sistema di vendita di libri online utilizzando socket in ambiente Unix/C. 

Il sistema è composto da tre tipi di processi:
1. **Server (server.c)**: Gestisce l'inventario dei libri, riceve richieste dai client e aggiorna lo stato.
2. **Case Editrici (publisher.c)**: Inviano nuove forniture di libri al server.
3. **Clienti Lettori (reader.c)**: Richiedono libri al server e attendono la disponibilità.

## Struttura della Cartella
```
libreria/
├── Makefile         # File per la compilazione
├── server.c         # Implementazione del server
├── publisher.c      # Client per l'inserimento di nuovi libri
├── reader.c         # Client per la richiesta di libri
├── README.md        # Documentazione del progetto
```

## Compilazione
Per compilare il progetto, eseguire:
```sh
make
```

Questo comando genererà i file eseguibili `server`, `publisher` e `reader`.

Per pulire i file compilati:
```sh
make clean
```

## Esecuzione
### Avvio del server
Prima di tutto, avviare il server:
```sh
./server &
```
Il server gestisce l'inventario dei libri e le richieste.

### Aggiunta di un libro (Case Editrici)
Un client editore può aggiungere un libro specificando il titolo e il numero di copie:
```sh
./publisher "LibroA" 5
```
Ciò aggiunge 5 copie di "LibroA" all'inventario.

### Acquisto di un libro (Client Lettore)
Un client lettore può richiedere un libro specificando il titolo:
```sh
./reader "LibroA"
```
Se il libro è disponibile, il client riceve una conferma e termina. Se il libro non è disponibile, rimane in attesa.

## Funzionamento del Server
- Mantiene una lista di libri disponibili.
- Quando riceve una fornitura da un editore, aggiorna l'inventario e soddisfa eventuali richieste in sospeso.
- Se un cliente richiede un libro non disponibile, rimane in attesa.
- Se un libro esaurisce le copie disponibili, viene rimosso dall'inventario.

## Note
- Il server utilizza il protocollo TCP per la comunicazione.
- La gestione della concorrenza è implementata con thread e mutex.

---
