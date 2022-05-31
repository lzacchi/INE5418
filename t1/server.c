#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#include "server.h"
#include "files.h"

#define MAX_CONNECTIONS 2

void *client_thread(void *args)
{
    int client_sockfd = *((int *)args);
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
        perror("Error opening socket\n");
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);

    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    int connections = MAX_CONNECTIONS;
    int requests = 0;
    int client_sockfd;
    unsigned int client_len;
    struct sockaddr_in client_address;

    listen(server_sockfd, connections);
    printf("Server loop started\n");
    while (TRUE)
    {
        client_len = sizeof(client_address);
        if (requests > connections)
        {
            printf("Error: max number of parallel connections reached\n");
            requests--;
            // reject connection
        }
        else
        {
            client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
            pthread_t cli_thread;
            pthread_create(&cli_thread, NULL, client_thread, (void *)&client_sockfd);
            requests++;
        }
    }
    close(server_sockfd);
    return 0;
}
