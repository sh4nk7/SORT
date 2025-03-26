#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include "common.h"
#include "utils.h"

typedef struct {
    char nome[MAX_NAME_LEN];
    int min, max, fd;
    int tot;
    int visitatori_fd[10];
    int visitatori_n[10];
    int num_gruppi;
} Guida;

Guida guide[10];
int num_guide = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* handle_client(void* arg) {
    int client_fd = *(int*)arg;
    free(arg);

    int type;
    recv(client_fd, &type, sizeof(int), 0);

    if (type == MSG_GUIDA) {
        GuidaMsg gmsg;
        recv(client_fd, &gmsg, sizeof(gmsg), 0);

        pthread_mutex_lock(&lock);
        Guida *g = &guide[num_guide++];
        strcpy(g->nome, gmsg.nome_guida);
        g->min = gmsg.min_visitatori;
        g->max = gmsg.max_visitatori;
        g->fd = client_fd;
        g->tot = 0;
        g->num_gruppi = 0;
        pthread_mutex_unlock(&lock);
        return NULL;
    }

    if (type == MSG_VISITATORI) {
        VisitatoriMsg vmsg;
        recv(client_fd, &vmsg, sizeof(vmsg), 0);

        pthread_mutex_lock(&lock);
        int best = -1;
        int min_spreco = 9999;

        for (int i = 0; i < num_guide; ++i) {
            int spazio = guide[i].max - guide[i].tot;
            if (spazio >= vmsg.num_visitatori && spazio < min_spreco) {
                min_spreco = spazio;
                best = i;
            }
        }

        if (best == -1) {
            int res = MSG_NO_GUIDE;
            send(client_fd, &res, sizeof(int), 0);
            close(client_fd);
        } else {
            Guida *g = &guide[best];
            g->visitatori_fd[g->num_gruppi] = client_fd;
            g->visitatori_n[g->num_gruppi] = vmsg.num_visitatori;
            g->tot += vmsg.num_visitatori;
            g->num_gruppi++;

            if (g->tot >= g->min) {
                StartMsg start = {MSG_OK, g->tot};
                send(g->fd, &start, sizeof(start), 0);
                close(g->fd);

                NomeGuidaMsg reply;
                reply.type = MSG_OK;
                strcpy(reply.nome_guida, g->nome);

                for (int j = 0; j < g->num_gruppi; ++j) {
                    send(g->visitatori_fd[j], &reply, sizeof(reply), 0);
                    close(g->visitatori_fd[j]);
                }

                for (int i = best; i < num_guide - 1; ++i)
                    guide[i] = guide[i + 1];
                num_guide--;
            }
        }

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    int server_fd = create_server_socket(SERVER_PORT);
    printf("[SERVER] In ascolto sulla porta %d\n", SERVER_PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
        if (*client_fd < 0) continue;

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_fd);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}
