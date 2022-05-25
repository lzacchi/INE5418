#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "server.h"

int server_listen(int *socket)
{
    int clients[N_CLIENTS];
    int connection = 0;
    while (1)
    {
        // new thtread for accep
    }
}

int main()
{
    int server_sockfd;
    int server_len, client_len;

    struct sockaddr_in server_address;

    char ch;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);

    server_len = sizeof(server_address);

    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    server_listen(&server_sockfd);

    // accept
}
