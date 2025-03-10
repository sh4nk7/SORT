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
