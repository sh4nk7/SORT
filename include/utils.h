#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int create_server_socket(int port);
int connect_to_server(const char *ip, int port);
void perror_exit(const char *msg);

#endif // UTILS_H
