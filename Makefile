CC = gcc
CFLAGS = -pthread -Wall
TARGETS = server publisher reader

all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) server.c -o server

publisher: publisher.c
	$(CC) $(CFLAGS) publisher.c -o publisher

reader: reader.c
	$(CC) $(CFLAGS) reader.c -o reader

clean:
	rm -f $(TARGETS)
