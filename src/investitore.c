#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    connect(sock, (struct sockaddr *)&server, sizeof(server));
    char buffer[1024] = "INVESTITORE";
    send(sock, buffer, strlen(buffer), 0);
    read(sock, buffer, sizeof(buffer));
    printf("Titoli disponibili:\n%s\n", buffer);
    printf("Inserire nome agente per acquisto: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    send(sock, buffer, strlen(buffer), 0);
    close(sock);
    return 0;
}