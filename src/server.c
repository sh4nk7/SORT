#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include "common.h"
#include "utils.h"

typedef struct {
    int client_fd;
    char sensor_id[SENSOR_ID_LEN];
    float value;
} SensorData;

typedef struct {
    char actuator_id[ACTUATOR_ID_LEN];
    float tgoal;
    int num_sensors;
    char sensor_ids[MAX_SENSORS_SUBSCRIBED][SENSOR_ID_LEN];
    int socket_fd;
} Actuator;

Actuator actuators[MAX_CLIENTS];
int actuator_count = 0;

void handle_measure(SensorData data) {
    for (int i = 0; i < actuator_count; ++i) {
        for (int j = 0; j < actuators[i].num_sensors; ++j) {
            if (strcmp(actuators[i].sensor_ids[j], data.sensor_id) == 0) {
                MeasureMsg msg = { MSG_TYPE_MEASURE, "", data.value };
                strcpy(msg.sensor_id, data.sensor_id);
                send(actuators[i].socket_fd, &msg, sizeof(msg), 0);
            }
        }
    }
    int count = 0;
    for (int i = 0; i < actuator_count; ++i)
        for (int j = 0; j < actuators[i].num_sensors; ++j)
            if (strcmp(actuators[i].sensor_ids[j], data.sensor_id) == 0)
                count++;
    write(data.client_fd, &count, sizeof(int));
}

int main() {
    int server_fd = create_server_socket(SERVER_PORT);
    printf("[SERVER] In ascolto sulla porta %d...\n", SERVER_PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
        if (client_fd < 0) continue;

        if (fork() == 0) {
            close(server_fd);
            int type;
            recv(client_fd, &type, sizeof(int), 0);

            if (type == MSG_TYPE_SUBSCRIBE) {
                SubscribeMsg msg;
                recv(client_fd, &msg, sizeof(msg), 0);
                printf("[SERVER] Iscrizione attuatore: %s\n", msg.actuator_id);
                strcpy(actuators[actuator_count].actuator_id, msg.actuator_id);
                actuators[actuator_count].tgoal = msg.tgoal;
                actuators[actuator_count].num_sensors = msg.num_sensors;
                actuators[actuator_count].socket_fd = client_fd;
                for (int i = 0; i < msg.num_sensors; ++i)
                    strcpy(actuators[actuator_count].sensor_ids[i], msg.sensor_ids[i]);
                actuator_count++;
                pause();
            } else if (type == MSG_TYPE_MEASURE) {
                MeasureMsg m;
                recv(client_fd, &m, sizeof(m), 0);
                SensorData d = { client_fd, "", m.value };
                strcpy(d.sensor_id, m.sensor_id);
                handle_measure(d);
                close(client_fd);
            } else if (type == MSG_TYPE_UNSUBSCRIBE) {
                UnsubscribeMsg m;
                recv(client_fd, &m, sizeof(m), 0);
                for (int i = 0; i < actuator_count; ++i) {
                    if (strcmp(actuators[i].actuator_id, m.actuator_id) == 0) {
                        close(actuators[i].socket_fd);
                        for (int j = i; j < actuator_count - 1; ++j)
                            actuators[j] = actuators[j + 1];
                        actuator_count--;
                        break;
                    }
                }
                close(client_fd);
            }

            exit(0);
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
