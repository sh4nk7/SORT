#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include "common.h"
#include "utils.h"

float random_temp() {
    return ((float)rand() / RAND_MAX) * 40.0 - 5.0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <sensor_id> <Nt>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *sensor_id = argv[1];
    int Nt = atoi(argv[2]);

    srand(time(NULL) ^ getpid());

    for (int i = 0; i < Nt; ++i) {
        if (fork() == 0) {
            sleep(rand() % 7); // 0-6 secondi
            int sock = connect_to_server("127.0.0.1", HUB_PORT);

            MeasureMsg msg;
            msg.type = MSG_TYPE_MEASURE;
            strncpy(msg.sensor_id, sensor_id, SENSOR_ID_LEN);
            msg.value = random_temp();

            send(sock, &msg, sizeof(msg), 0);

            int received;
            read(sock, &received, sizeof(int));
            printf("[SENSORE %s] Misura %.2f°C ricevuta da %d attuatori\n", sensor_id, msg.value, received);

            close(sock);
            exit(0);
        }
    }

    for (int i = 0; i < Nt; ++i)
        wait(NULL);

    return 0;
}

