#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_TITOLI 100
#define MAX_NAME 30
#define PORT 8080

typedef struct {
    char nome[MAX_NAME];
    int quantita;
    int prezzo_unitario;
    int prezzo_minimo;
    int ricavo;
} Titolo;

Titolo titoli[MAX_TITOLI];
int num_titoli = 0;
pthread_mutex_t lock;

void *gestisci_client(void *socket_desc);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    pthread_mutex_init(&lock, NULL);

    printf("Server in ascolto sulla porta %d...\n", PORT);
    
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        pthread_t thread;
        pthread_create(&thread, NULL, gestisci_client, (void*)&new_socket);
    }
    
    return 0;
}

void *gestisci_client(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char buffer[1024] = {0};
    read(sock, buffer, sizeof(buffer));
    
    if (strncmp(buffer, "AGENTE", 6) == 0) {
        Titolo nuovo_titolo;
        sscanf(buffer + 7, "%s %d %d %d", nuovo_titolo.nome, &nuovo_titolo.quantita, &nuovo_titolo.prezzo_unitario, &nuovo_titolo.prezzo_minimo);
        nuovo_titolo.ricavo = 0;
        pthread_mutex_lock(&lock);
        titoli[num_titoli++] = nuovo_titolo;
        pthread_mutex_unlock(&lock);
    } else if (strncmp(buffer, "INVESTITORE", 11) == 0) {
        send(sock, titoli, sizeof(titoli), 0);
        read(sock, buffer, sizeof(buffer));
        pthread_mutex_lock(&lock);
        for (int i = 0; i < num_titoli; i++) {
            if (strcmp(titoli[i].nome, buffer) == 0) {
                titoli[i].quantita--;
                titoli[i].prezzo_unitario++;
                titoli[i].ricavo += titoli[i].prezzo_unitario;
                if (titoli[i].quantita == 0 || titoli[i].prezzo_unitario < titoli[i].prezzo_minimo) {
                    printf("Titolo %s ritirato. Ricavo: %d\n", titoli[i].nome, titoli[i].ricavo);
                    for (int j = i; j < num_titoli - 1; j++) {
                        titoli[j] = titoli[j + 1];
                    }
                    num_titoli--;
                }
                break;
            }
        }
        pthread_mutex_unlock(&lock);
    }
    close(sock);
    return NULL;
}
