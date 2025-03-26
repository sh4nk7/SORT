#ifndef COMMON_H
#define COMMON_H

#define SERVER_PORT 12347
#define MAX_NAME_LEN 30
#define BUFFER_SIZE 1024

#define MSG_GUIDA 1
#define MSG_VISITATORI 2
#define MSG_OK 3
#define MSG_NO_GUIDE 4

typedef struct {
    int type;
    char nome_guida[MAX_NAME_LEN];
    int min_visitatori;
    int max_visitatori;
} GuidaMsg;

typedef struct {
    int type;
    int num_visitatori;
} VisitatoriMsg;

typedef struct {
    int type;
    int totale;
} StartMsg;

typedef struct {
    int type;
    char nome_guida[MAX_NAME_LEN];
} NomeGuidaMsg;

#endif
