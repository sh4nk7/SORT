/* Client Giocatore */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char name[50], result[100];
    
    // Creazione socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Errore nella creazione della socket");
        exit(EXIT_FAILURE);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    // Connessione al server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Errore di connessione al server");
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    printf("Inserisci il tuo nome: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // Rimuove il carattere di nuova riga
    
    // Invia il nome al server
    if (write(sock, name, strlen(name) + 1) == -1) {
        perror("Errore nell'invio del nome al server");
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    // Attendi il risultato della partita
    if (read(sock, result, sizeof(result)) == -1) {
        perror("Errore nella ricezione del risultato");
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    printf("Risultato partita: %s\n", result);
    
    close(sock);
    return 0;
}

