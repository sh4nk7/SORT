#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

#define PORT 8080
#define MAX_BOOKS 100
#define MAX_PENDING 100

typedef struct {
    char title[20];
    int copies;
} Book;

Book inventory[MAX_BOOKS];
int book_count = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void handle_client(int client_sock) {
    char buffer[1024];
    read(client_sock, buffer, sizeof(buffer));
    
    char command[10], title[20];
    int quantity;
    sscanf(buffer, "%s %s %d", command, title, &quantity);
    
    pthread_mutex_lock(&lock);
    if (strcmp(command, "BUY") == 0) {
        for (int i = 0; i < book_count; i++) {
            if (strcmp(inventory[i].title, title) == 0 && inventory[i].copies > 0) {
                inventory[i].copies--;
                write(client_sock, "AVAILABLE", 10);
                close(client_sock);
                pthread_mutex_unlock(&lock);
                return;
            }
        }
        write(client_sock, "WAIT", 5);
    } else if (strcmp(command, "ADD") == 0) {
        for (int i = 0; i < book_count; i++) {
            if (strcmp(inventory[i].title, title) == 0) {
                pthread_mutex_unlock(&lock);
                close(client_sock);
                return;
            }
        }
        strcpy(inventory[book_count].title, title);
        inventory[book_count].copies = quantity;
        book_count++;
        printf("Book added: %s (%d copies)\n", title, quantity);
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
    listen(server_sock, MAX_PENDING);
    
    printf("Server started...\n");
    while ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len))) {
        pthread_t thread;
        pthread_create(&thread, NULL, (void *)handle_client, (void *)(intptr_t)client_sock);
        pthread_detach(thread);
    }
    close(server_sock);
    return 0;
}
