#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <nome_guida> <min> <max>\n", argv[0]);
        exit(1);
    }

    int sock = connect_to_server("127.0.0.1", SERVER_PORT);

    int type = MSG_GUIDA;
    send(sock, &type, sizeof(int), 0);

    GuidaMsg msg;
    msg.type = MSG_GUIDA;
    strncpy(msg.nome_guida, argv[1], MAX_NAME_LEN);
    msg.min_visitatori = atoi(argv[2]);
    msg.max_visitatori = atoi(argv[3]);

    send(sock, &msg, sizeof(msg), 0);

    StartMsg risposta;
    recv(sock, &risposta, sizeof(risposta), 0);

    printf("[GUIDA %s] Inizia visita con %d visitatori.\n", argv[1], risposta.totale);
    close(sock);
    return 0;
}
