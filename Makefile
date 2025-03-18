CC = gcc
CFLAGS = -pthread -Wall
TARGETS = server centro fornitore

all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) server.c -o server

centro: centro.c
	$(CC) $(CFLAGS) centro.c -o centro

fornitore: fornitore.c
	$(CC) $(CFLAGS) fornitore.c -o fornitore

clean:
	rm -f $(TARGETS)
