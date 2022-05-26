#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "server.h"

int server_listen(int *socket)
{
    int server_sockfd = *socket;
    int connections = N_CLIENTS;
    int requests = 0;
    int clients[connections];
    unsigned int client_len;
    struct sockaddr_in client_address;

    listen(server_sockfd, connections);
    printf("Listening to max number of clients: %d\n", connections);
    while (1)
    {
        printf("Server listening\n");
        client_len = sizeof(client_address);

        if (requests == (connections - 1))
        {
            printf("Reached max number of connected clients\n");
        }
        else
        {
            clients[requests] = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
            pthread_t cli_thread;

            pthread_create(&cli_thread, NULL, client_thread, (void *)clients[requests]);
            requests++;
        }
        requests--;
    }
    return 0;
}

void *client_thread(void *args)
{
    int client_sockfd = *((int *)args);
    char filename;
    while (1)
    {
        read(client_sockfd, &filename, 1);
        write(client_sockfd, "Error: no such file or directory\n", 1);
        sleep(10);
        break;
    }
    close(client_sockfd);
    return 0;
}

int main()
{
    int server_sockfd;
    int server_len;

    struct sockaddr_in server_address;

    char ch;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);

    server_len = sizeof(server_address);

    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    server_listen(&server_sockfd);
    close(server_sockfd);
    exit(0);

    return 0;
}
