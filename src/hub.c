#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "common.h"
#include "utils.h"

#define MAX_SENSORS 100

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <J>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int J = atoi(argv[1]);
    int server_fd = create_server_socket(HUB_PORT);
    printf("[HUB] In ascolto su porta %d, attesa %d sensori...\n", HUB_PORT, J);

    int sensor_fds[MAX_SENSORS];
    int sensor_count = 0;

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
        if (client_fd < 0) continue;

        sensor_fds[sensor_count++] = client_fd;

        if (sensor_count >= J) {
            for (int i = 0; i < sensor_count; ++i) {
                if (fork() == 0) {
                    int hub_sock = connect_to_server("127.0.0.1", SERVER_PORT);

                    int type = MSG_TYPE_MEASURE;
                    send(hub_sock, &type, sizeof(int), 0);

                    MeasureMsg msg;
                    recv(sensor_fds[i], &msg, sizeof(msg), 0);
                    send(hub_sock, &msg, sizeof(msg), 0);

                    int received;
                    read(hub_sock, &received, sizeof(int));
                    write(sensor_fds[i], &received, sizeof(int));

                    close(hub_sock);
                    close(sensor_fds[i]);
                    exit(0);
                }
                close(sensor_fds[i]);
            }
            sensor_count = 0;
        }
    }

    close(server_fd);
    return 0;
}

