# Sistema di Distribuzione Vaccini

## Descrizione
Questo progetto implementa un sistema di distribuzione di vaccini tra un server, centri vaccinali e fornitori, utilizzando socket TCP in ambiente Unix/C.

Il sistema è composto da tre componenti:
1. **Server (`server.c`)**: Gestisce le richieste di vaccini dai centri e le offerte dai fornitori.
2. **Centri Vaccinali (`centro.c`)**: Richiedono vaccini al server e attendono la fornitura.
3. **Fornitori (`fornitore.c`)**: Offrono vaccini e attendono di essere abbinati ai centri vaccinali.

## Struttura della Cartella
```
centri_vaccinali/
├── Makefile         # File per la compilazione
├── server.c         # Implementazione del server
├── centro.c         # Client per i centri vaccinali
├── fornitore.c      # Client per i fornitori di vaccini
├── README.md        # Documentazione del progetto
```

## Compilazione
Per compilare il progetto, eseguire:
```sh
make
```
Questo comando genererà gli eseguibili `server`, `centro` e `fornitore`.

Per pulire i file compilati:
```sh
make clean
```

## Esecuzione
### Avvio del server
Avviare il server prima degli altri processi:
```sh
./server &
```

### Richiesta di vaccini da un centro vaccinale
Un centro vaccinale può registrarsi e specificare la quantità di vaccini richiesta:
```sh
./centro "CentroA" 10
```

### Fornitura di vaccini da un fornitore
Un fornitore può registrarsi e specificare la quantità di vaccini disponibili e il minimo necessario per avviare la distribuzione:
```sh
./fornitore "FornitoreX" 20 5
```

Il server abbinerà automaticamente i fornitori ai centri utilizzando una politica greedy.

## Funzionamento del Server
- Accetta connessioni da centri e fornitori.
- Accumula richieste di centri e offerte di fornitori.
- Quando un fornitore si connette, verifica se esiste un numero sufficiente di richieste per iniziare la distribuzione.
- Se sì, distribuisce i vaccini in modo greedy ai centri con richieste più alte fino a esaurire la scorta disponibile.
- Notifica i centri riforniti e il fornitore.

## Note
- Il server utilizza thread per gestire più connessioni simultanee.
- Le connessioni avvengono tramite socket TCP.
- L'allocazione dei vaccini avviene seguendo una strategia greedy.
