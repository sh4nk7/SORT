#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <num_visitatori>\n", argv[0]);
        exit(1);
    }

    int sock = connect_to_server("127.0.0.1", SERVER_PORT);

    int type = MSG_VISITATORI;
    send(sock, &type, sizeof(int), 0);

    VisitatoriMsg msg;
    msg.type = MSG_VISITATORI;
    msg.num_visitatori = atoi(argv[1]);

    send(sock, &msg, sizeof(msg), 0);

    int risposta_type;
    recv(sock, &risposta_type, sizeof(int), 0);

    if (risposta_type == MSG_NO_GUIDE) {
        printf("[VISITATORI] Nessuna guida disponibile.\n");
    } else {
        NomeGuidaMsg risposta;
        recv(sock, &risposta, sizeof(risposta), 0);
        printf("[VISITATORI] In visita con guida %s.\n", risposta.nome_guida);
    }

    close(sock);
    return 0;
}
