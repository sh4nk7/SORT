#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "common.h"
#include "utils.h"

float history[MAX_SENSORS_SUBSCRIBED][MAX_MEASURE_HISTORY];
int count[MAX_SENSORS_SUBSCRIBED];

float media(float *arr, int n) {
    float sum = 0.0;
    for (int i = 0; i < n; ++i) sum += arr[i];
    return sum / n;
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        int sock = connect_to_server("127.0.0.1", SERVER_PORT);
        int type = MSG_TYPE_UNSUBSCRIBE;
        send(sock, &type, sizeof(int), 0);

        UnsubscribeMsg msg;
        msg.type = type;
        strncpy(msg.actuator_id, argv[1], ACTUATOR_ID_LEN);
        send(sock, &msg, sizeof(msg), 0);
        close(sock);
        printf("[ATTUATORE %s] Disiscritto.\n", argv[1]);
        return 0;
    }

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <id> <Tgoal> <sensor1> [sensor2 ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *id = argv[1];
    float Tgoal = atof(argv[2]);
    int num_sensors = argc - 3;

    int sock = connect_to_server("127.0.0.1", SERVER_PORT);
    int type = MSG_TYPE_SUBSCRIBE;
    send(sock, &type, sizeof(int), 0);

    SubscribeMsg msg;
    msg.type = type;
    strncpy(msg.actuator_id, id, ACTUATOR_ID_LEN);
    msg.tgoal = Tgoal;
    msg.num_sensors = num_sensors;
    for (int i = 0; i < num_sensors; ++i)
        strncpy(msg.sensor_ids[i], argv[i + 3], SENSOR_ID_LEN);

    send(sock, &msg, sizeof(msg), 0);
    printf("[ATTUATORE %s] Sottoscritto con Tgoal=%.2f\n", id, Tgoal);

    while (1) {
        MeasureMsg m;
        int r = recv(sock, &m, sizeof(m), 0);
        if (r <= 0 || strlen(m.sensor_id) == 0) break;

        for (int i = 0; i < num_sensors; ++i) {
            if (strcmp(m.sensor_id, msg.sensor_ids[i]) == 0) {
                if (count[i] < MAX_MEASURE_HISTORY) {
                    history[i][count[i]++] = m.value;
                } else {
                    for (int j = 1; j < MAX_MEASURE_HISTORY; ++j)
                        history[i][j - 1] = history[i][j];
                    history[i][MAX_MEASURE_HISTORY - 1] = m.value;
                }
            }
        }

        int below = 0, total = 0;
        for (int i = 0; i < num_sensors; ++i) {
            if (count[i] >= 3) {
                float avg = media(history[i], count[i]);
                if (avg < Tgoal) below++;
                total++;
            }
        }

        if (total > 0) {
            if (below > total / 2)
                printf("[ATTUATORE %s] Accendi riscaldamento\n", id);
            else
                printf("[ATTUATORE %s] Spegni riscaldamento\n", id);
        }
    }

    close(sock);
    return 0;
}

