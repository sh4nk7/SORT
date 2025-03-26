#ifndef COMMON_H
#define COMMON_H

#define SERVER_PORT 12345
#define HUB_PORT 12346
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024
#define SENSOR_ID_LEN 32
#define ACTUATOR_ID_LEN 32
#define MAX_SENSORS_SUBSCRIBED 10
#define MAX_MEASURE_HISTORY 10

// Messaggi tra processi
#define MSG_TYPE_MEASURE 1
#define MSG_TYPE_SUBSCRIBE 2
#define MSG_TYPE_UNSUBSCRIBE 3
#define MSG_TYPE_TERMINATE 4

typedef struct {
    int type;
    char sensor_id[SENSOR_ID_LEN];
    float value;
} MeasureMsg;

typedef struct {
    int type;
    char actuator_id[ACTUATOR_ID_LEN];
    float tgoal;
    int num_sensors;
    char sensor_ids[MAX_SENSORS_SUBSCRIBED][SENSOR_ID_LEN];
} SubscribeMsg;

typedef struct {
    int type;
    char actuator_id[ACTUATOR_ID_LEN];
} UnsubscribeMsg;

#endif // COMMON_H
