#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/wait.h>

#define PORT 8080
#define NUM_REQUESTS 5  // Numero di richieste concorrenti

void client_task(int id) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL) + getpid());  // Genera numero casuale unico
    int num = rand() % 100;
    printf("Client %d sending: %d\n", id, num);

    send(sock, &num, sizeof(num), 0);
    read(sock, &num, sizeof(num));
    printf("Client %d received: %d\n", id, num);

    close(sock);
}

int main() {
    for (int i = 0; i < NUM_REQUESTS; i++) {
        pid_t pid = fork();
        if (pid == 0) {  // Figlio
            client_task(i);
            exit(0);
        }
    }

    // Aspetta la terminazione di tutti i figli
    while (wait(NULL) > 0);

    return 0;
}
