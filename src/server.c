/* Server Gioco Multi-Utente */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>



#define MAX_PLAYERS 100
#define PORT 8080
#define N 3

// Struttura per il profilo del giocatore
typedef struct {
    char name[50];
    int games_played;
    int score;
} Player;

Player players[MAX_PLAYERS];
int player_count = 0;
int connected_players = 0;
int active_sockets[N];
pthread_mutex_t lock;

// Aggiungi o trova un giocatore
int find_or_add_player(char *name) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < player_count; i++) {
        if (strcmp(players[i].name, name) == 0) {
            pthread_mutex_unlock(&lock);
            return i;
        }
    }
    if (player_count < MAX_PLAYERS) {
        strcpy(players[player_count].name, name);
        players[player_count].games_played = 0;
        players[player_count].score = 0;
        player_count++;
    }
    pthread_mutex_unlock(&lock);
    return player_count - 1;
}

// Simula la partita e assegna punteggi
void simulate_game() {
    pthread_mutex_lock(&lock);
    printf("Simulazione partita...\n");
    int indices[N];
    for (int i = 0; i < N; i++) indices[i] = i;
    
    // Shuffle casuale
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        int j = rand() % N;
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
    
    // Assegna punteggi ai primi 3 classificati
    players[indices[0]].score += 3;
    players[indices[1]].score += 2;
    players[indices[2]].score += 1;
    for (int i = 0; i < N; i++) players[indices[i]].games_played++;
    
    // Invia risultati ai giocatori
    char result[200];  // Buffer più grande per evitare troncamenti
    snprintf(result, sizeof(result), "Classifica: 1° %.30s, 2° %.30s, 3° %.30s",  
             players[indices[0]].name, players[indices[1]].name, players[indices[2]].name);
    for (int i = 0; i < N; i++) {
        write(active_sockets[i], result, strlen(result) + 1);
        close(active_sockets[i]);
    }
    connected_players = 0;
    pthread_mutex_unlock(&lock);
}

// Gestisce il client
void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    free(arg);
    char name[50];
    read(client_socket, name, sizeof(name));
    
    int index = find_or_add_player(name);
    
    pthread_mutex_lock(&lock);
    active_sockets[connected_players++] = client_socket;
    printf("Giocatore connesso: %s (%d partite, %d punti)\n", 
           players[index].name, players[index].games_played, players[index].score);
    
    if (connected_players == N) {
        simulate_game();
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 10);
    
    pthread_mutex_init(&lock, NULL);
    printf("Server in ascolto sulla porta %d...\n", PORT);
    
    while (1) {
        addr_size = sizeof(client_addr);
        new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        int *client_sock = malloc(sizeof(int));
        *client_sock = new_socket;
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_sock);
        pthread_detach(tid);
    }
    
    close(server_socket);
    pthread_mutex_destroy(&lock);
    return 0;
}

