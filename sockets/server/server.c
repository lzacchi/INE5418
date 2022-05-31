#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>

#include "server.h"
#include "../file_handler/file_handler.h"

int connected_clients = 0;
pthread_mutex_t mutex;

int server_listen(int *socket)
{
    int server_sockfd = *socket;
    int connections = N_CLIENTS;
    int client_sockfd;
    unsigned int client_len;
    struct sockaddr_in client_address;

    listen(server_sockfd, connections);
    printf("Server listening\n");
    printf("Max number of clients: %d\n", connections);
    while (1)
    {
        client_len = sizeof(client_address);
        if (connected_clients == (connections))
        {
            printf("Reached max number of connected clients\n");
        }
        else
        {
            client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
            pthread_t client;
            pthread_create(&client, NULL, client_handler, (void *)&client_sockfd);
        }
    }
    return 0;
}

void increment_client_counter()
{
    pthread_mutex_lock(&mutex);
    ++connected_clients;
    pthread_mutex_unlock(&mutex);
}

void decrement_client_counter()
{
    pthread_mutex_lock(&mutex);
    --connected_clients;
    pthread_mutex_unlock(&mutex);
}

void *client_handler(void *client_socket)
{
    int client_sockfd = *((int *)client_socket);
    increment_client_counter();
    printf("created thread\n");
    while (1)
    {
        int str_len = 0;
        read(client_sockfd, &str_len, sizeof(int));

        char filename[str_len];
        read(client_sockfd, &filename, str_len);
        printf("File name is '%s'\n", filename);

        char *file = read_file(filename);
        int filesize = malloc_usable_size(file);
        write(client_sockfd, &filesize, sizeof(int));
        printf("File Size is %i bytes\n", filesize);

        printf("Sending file...\n");
        write(client_sockfd, file, filesize);
        printf("Finished sending file\n");
        sleep(10);
        break;
    }
    close(client_sockfd);
    decrement_client_counter();
    return 0;
}

int main()
{
    int server_sockfd;
    int server_len;

    struct sockaddr_in server_address;

    char ch;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sockfd < 0)
    {
        printf("Error opening socket\n");
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(9734);

    server_len = sizeof(server_address);

    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    server_listen(&server_sockfd);
    close(server_sockfd);
    exit(0);

    return 0;
}
