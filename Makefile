all:
	gcc -o bin/server src/server.c -pthread
	gcc -o bin/agente src/agente.c
	gcc -o bin/investitore src/investitore.c

clean:
	rm -f bin/server bin/agente bin/investitore
