#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s <nome> <quantita> <prezzo_unitario> <prezzo_minimo>\n", argv[0]);
        return 1;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    connect(sock, (struct sockaddr *)&server, sizeof(server));
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "AGENTE %s %s %s %s", argv[1], argv[2], argv[3], argv[4]);
    send(sock, buffer, strlen(buffer), 0);
    close(sock);
    return 0;
}