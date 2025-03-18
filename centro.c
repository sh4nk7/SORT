#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

void register_centro(const char *nome, int richiesta) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    char buffer[1024];
    sprintf(buffer, "CENTRO %s %d", nome, richiesta);
    write(sock, buffer, strlen(buffer) + 1);
    close(sock);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <Centro_Nome> <Numero_Vaccini>\n", argv[0]);
        return 1;
    }
    register_centro(argv[1], atoi(argv[2]));
    return 0;
}
