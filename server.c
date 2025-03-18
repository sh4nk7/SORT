#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

#define PORT 8080
#define MAX_CENTRI 100
#define MAX_FORNITORI 100

typedef struct {
    char nome[20];
    int richiesta;
} Centro;

typedef struct {
    char nome[20];
    int vaccini_disponibili;
    int minimo_richiesto;
} Fornitore;

Centro centri[MAX_CENTRI];
int num_centri = 0;
Fornitore fornitori[MAX_FORNITORI];
int num_fornitori = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void handle_client(int client_sock) {
    char buffer[1024];
    read(client_sock, buffer, sizeof(buffer));
    
    char tipo[10], nome[20];
    int valore1, valore2;
    sscanf(buffer, "%s %s %d %d", tipo, nome, &valore1, &valore2);
    
    pthread_mutex_lock(&lock);
    if (strcmp(tipo, "CENTRO") == 0) {
        strcpy(centri[num_centri].nome, nome);
        centri[num_centri].richiesta = valore1;
        num_centri++;
    } else if (strcmp(tipo, "FORNITORE") == 0) {
        strcpy(fornitori[num_fornitori].nome, nome);
        fornitori[num_fornitori].vaccini_disponibili = valore1;
        fornitori[num_fornitori].minimo_richiesto = valore2;
        num_fornitori++;
    }
    pthread_mutex_unlock(&lock);
    close(client_sock);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, MAX_CENTRI + MAX_FORNITORI);
    
    printf("Server started...\n");
    while ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len))) {
        pthread_t thread;
        pthread_create(&thread, NULL, (void *)handle_client, (void *)(intptr_t)client_sock);
        pthread_detach(thread);
    }
    close(server_sock);
    return 0;
}
