CC=gcc
CFLAGS=-Wall -Iinclude

SRC=src
BIN=bin

all: $(BIN)/server $(BIN)/hub $(BIN)/sensore $(BIN)/attuatore

$(BIN)/server: $(SRC)/server.c $(SRC)/utils.c
	$(CC) $(CFLAGS) $^ -o $@

$(BIN)/hub: $(SRC)/hub.c $(SRC)/utils.c
	$(CC) $(CFLAGS) $^ -o $@

$(BIN)/sensore: $(SRC)/sensore.c $(SRC)/utils.c
	$(CC) $(CFLAGS) $^ -o $@

$(BIN)/attuatore: $(SRC)/attuatore.c $(SRC)/utils.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(BIN)/*
