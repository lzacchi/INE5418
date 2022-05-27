#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "server.h"
#include "files.h"

int server_listen(int *socket)
{
    int server_sockfd = *socket;
    int connections = N_CLIENTS;
    int requests = 0;
    int client_sockfd;
    unsigned int client_len;
    struct sockaddr_in client_address;

    listen(server_sockfd, connections);
    printf("Server listening\n");
    printf("Max number of clients: %d\n", connections);
    while (1)
    {
        client_len = sizeof(client_address);

        if (requests == (connections - 1))
        {
            printf("Reached max number of connected clients\n");
        }
        else
        {
            client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
            pthread_t cli_thread;

            pthread_create(&cli_thread, NULL, client_thread, (void *)&client_sockfd);
            requests++;
        }
        requests--;
    }
    return 0;
}

void *client_thread(void *args)
{
    int client_sockfd = *((int *)args);
    printf("created thread\n");
    while (1)
    {
        int str_len = 0;
        read(client_sockfd, &str_len, sizeof(int));

        char filename[str_len];
        read(client_sockfd, &filename, sizeof(filename));
        printf("File name is '%s'\n", filename);

        long filesize = 0;
        char *file = read_file(filename, &filesize);
        write(client_sockfd, &filesize, sizeof(long));
        printf("File Size is %ld bytes\n", filesize);

        printf("Sending file...\n");
        write(client_sockfd, file, filesize);
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
