#include "utils.h"

int create_server_socket(int port) {
    int sockfd;
    struct sockaddr_in serv_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket()");

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        perror_exit("bind()");

    if (listen(sockfd, 10) < 0)
        perror_exit("listen()");

    return sockfd;
}

int connect_to_server(const char *ip, int port) {
    int sockfd;
    struct sockaddr_in serv_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket()");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serv_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        perror_exit("connect()");

    return sockfd;
}

void perror_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
