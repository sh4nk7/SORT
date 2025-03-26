CC=gcc
CFLAGS=-Wall -Iinclude
SRC=src
BIN=bin

all: $(BIN)/server $(BIN)/guida $(BIN)/visitatori

$(BIN)/server: $(SRC)/server.c $(SRC)/utils.c
	$(CC) $(CFLAGS) $^ -o $@

$(BIN)/guida: $(SRC)/guida.c $(SRC)/utils.c
	$(CC) $(CFLAGS) $^ -o $@

$(BIN)/visitatori: $(SRC)/visitatori.c $(SRC)/utils.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(BIN)/*
