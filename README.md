# Esercitazione 1 – Programming Tools in GNU/Linux
Corso: Sistemi Operativi e in Tempo Reale  
A.A. 2024/2025  
Studente: Giuseppe Dimonte  

---

# 1. Obiettivo dell’Esercizio

L’esercizio richiede di:

- Correggere errori in un progetto multi-file
- Separare dichiarazione e definizione delle funzioni
- Evitare multiple definition error tramite include guards
- Creare una libreria statica a partire dai file ausiliari
- Realizzare un Makefile completo con target `all` e `clean`

---

# 2. Struttura del Progetto
```
esercitazione1/
├── main.c
├── fast_trigo.c
├── fast_trigo.h
├── Makefile
└── README.md
```

---

# 3. Problema Iniziale

Nel progetto originale si verificavano:

- Errori di ridefinizione delle funzioni
- Inclusioni multiple dell’header
- Mancata modularizzazione tramite libreria

Errore tipico in fase di linking:

multiple definition of `fast_sin'


Questo avviene quando una funzione viene definita più volte in file differenti o inclusa impropriamente tramite header.

---

# 4. Correzione Strutturale

## fast_trigo.h

- Inseriti include guards
- Presenti solo prototipi di funzione

```c
#ifndef FAST_TRIGO_H
#define FAST_TRIGO_H

#include <math.h>

float fast_sin(float x);
float fast_cos(float x);

#endif
```

##fast_trigo.c

Contiene solo le implementazioni:
```
#include "fast_trigo.h"

float fast_sin(float x) {
    return sinf(x);
}

float fast_cos(float x) {
    return cosf(x);
}
```
##main.c
Contiene esclusivamente le implementazioni delle funzioni.
```C 
#include <stdio.h>
#include "fast_trigo.h"

int main() {
    float angle = 3.14f / 4.0f;

    printf("sin(%f) = %f\n", angle, fast_sin(angle));
    printf("cos(%f) = %f\n", angle, fast_cos(angle));

    return 0;
}

```


#5. Processo di Build
Il processo completo è: 
```


main.c          fast_trigo.c
   │                 │
   ▼                 ▼
main.o        fast_trigo.o
                     │
                     ▼
              libfast_trigo.a
                     │
                     ▼
                  main

```

#6. Creazione Manuale della Libreria

## 6.1 Compilazione del file oggetto

```
gcc -Wall -Wextra -std=c11 -O2 -c fast_trigo.c

```

## 6.2 Creazione della libreria statica

```
ar rcs libfast_trigo.a fast_trigo.o

```

## 6.3 Linking con il programma principale

```
gcc main.c -L. -lfast_trigo -lm -o main

```

---

## Spiegazione dei flag di linking

Nel comando:

gcc main.c -L. -lfast_trigo -lm -o main

i flag utilizzati hanno il seguente significato:

### -L.

Indica al linker di cercare le librerie nella directory corrente (`.`).

Per impostazione predefinita, il linker cerca le librerie solo nelle directory standard di sistema, come:

- /usr/lib  
- /lib  
- /usr/local/lib  

Poiché la libreria `libfast_trigo.a` si trova nella directory del progetto, è necessario specificare esplicitamente il percorso con `-L.`.

---

### -lfast_trigo

Indica di collegare la libreria chiamata `libfast_trigo.a`.

Il linker applica automaticamente la convenzione:

- Prefisso `lib`
- Estensione `.a` (per libreria statica) oppure `.so` (per libreria dinamica)

Quindi:

-lfast_trigo

viene tradotto automaticamente in:

libfast_trigo.a

---

### -lm

Collega la libreria matematica standard (`libm`).

È necessaria perché le funzioni:

- sinf()
- cosf()

non fanno parte della libreria C standard (`libc`), ma della libreria matematica separata (`libm`).

Se si omette `-lm`, il linker genera un errore del tipo:

undefined reference to `sinf`

---

## Perché l’ordine dei flag è importante

Nel linking con `gcc`, l’ordine dei parametri conta.

Il linker analizza i file da sinistra verso destra e risolve i simboli nel momento in cui li incontra.

Per questo motivo:

gcc main.c -L. -lfast_trigo -lm -o main

è corretto, mentre invertire l’ordine delle librerie può causare errori di simboli non risolti.

Regola generale:

- Prima si mettono i file sorgente o oggetto
- Poi le librerie da cui dipendono

---

## Riepilogo

| Flag | Significato |
|------|------------|
| -L. | Cerca librerie nella directory corrente |
| -lfast_trigo | Collega libfast_trigo.a |
| -lm | Collega la libreria matematica libm |

---

# 7. Makefile

```
---

# Nome del compilatore
CC = gcc

# Flags per la compilazione
CFLAGS = -Wall -Wextra -std=c11 -O2

# Nome della libreria statica
LIB_NAME = libfast_trigo.a

# File oggetto
OBJ = fast_trigo.o

# Target principale
all: main

# Compilazione del file oggetto
$(OBJ): fast_trigo.c fast_trigo.h
	$(CC) $(CFLAGS) -c fast_trigo.c -o fast_trigo.o

# Creazione della libreria statica
$(LIB_NAME): $(OBJ)
	ar rcs $(LIB_NAME) $(OBJ)

# Compilazione del programma principale e link con la libreria
main: main.c $(LIB_NAME)
	$(CC) $(CFLAGS) main.c -L. -lfast_trigo -o main -lm

# Pulizia dei file oggetto e binari
clean: 
	rm -f *.o *.a main


```
---
# 8. Compilazione ed Esecuzione

Per compilare:

```
make

```

Per eseguire: 

```
./main

```

Output atteso:

```
sin(0.785000) = 0.707107
cos(0.785000) = 0.707107
```
Per pulire:
```
make clean
```
---

#9. Analisi dei Simboli

##Verifica simboli del file oggetto:
```
nm fast_trigo.o
```
Output tipico:
```
00000000 T fast_sin
00000010 T fast_cos
         U sinf
         U cosf
```
---
## Significato delle lettere

- **T** → simbolo definito nel file (presente nella sezione `.text`)
- **U** → simbolo undefined (non definito nel file oggetto)

Un simbolo `U` significa che la funzione viene usata ma non è definita in quel file.  
In questo caso `sinf` e `cosf` vengono risolti dal linker tramite il flag `-lm`, che collega la libreria matematica (`libm`).

---


## Linking Statico vs Linking Dinamico

### Linking Statico

- Il codice delle librerie viene incorporato direttamente nell’eseguibile
- L’eseguibile è più grande
- Non dipende da librerie esterne a runtime
- Maggiore prevedibilità
- Maggiore controllo sul comportamento del sistema

---

### Linking Dinamico

- Le librerie vengono caricate a runtime dal loader
- L’eseguibile è più piccolo
- Dipende da librerie esterne presenti nel sistema
- Minore footprint iniziale
- Leggera indeterminazione dovuta al caricamento dinamico

---

## Collegamento ai Sistemi Real-Time

Nei sistemi real-time è fondamentale:

- Garantire prevedibilità temporale
- Ridurre dipendenze dinamiche
- Controllare il footprint del binario
- Evitare comportamenti non deterministici

Il linking statico è generalmente preferibile in ambito real-time perché:

- Riduce dipendenze esterne
- Elimina il caricamento dinamico a runtime
- Aumenta la determinazione del sistema

Il linking dinamico, invece, introduce dipendenze dal loader e dalle librerie condivise, che possono influire sulla prevedibilità temporale.

---

## Cosa ho imparato da questo esercizio

Questo esercizio mi ha permesso di comprendere in modo concreto:

- La struttura di un progetto multi-file
- La separazione tra header (.h) e implementazione (.c)
- Il processo completo di compilazione in C
- La gestione dei simboli e del linking
- La creazione di una libreria statica con `ar`
- L’automazione della build tramite Makefile

La soluzione finale è conforme alla traccia del docente ed è correttamente modularizzata.

---

## 👨‍🎓 Informazioni accademiche

**Studente:** Dimonte Giuseppe  
**Numero di matricola:** 367431  
**Docente di riferimento:** Prof. Stefano Caselli --> stefano.caselli@unipr.it
**Tutor laboratorio:**Gabriele Penzotti   --> gabriele.penzotti@unipr.it
**Insegnamento:** SISTEMI OPERATIVI E IN TEMPO REALE  
**Corso di laurea:** Laurea Magistrale in Ingegneria Informatica  
**Percorso:** Intelligenza Artificiale
