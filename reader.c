#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

void request_book(const char *title) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    char buffer[1024];
    sprintf(buffer, "BUY %s", title);
    write(sock, buffer, strlen(buffer) + 1);
    read(sock, buffer, sizeof(buffer));
    printf("Server response: %s\n", buffer);
    close(sock);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <book_title>\n", argv[0]);
        return 1;
    }
    request_book(argv[1]);
    return 0;
}
