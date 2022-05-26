#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "client.h"

int main()
{
    char IP_ADDR[] = "0.0.0.0";

    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;

    char filename[] = "README.txt";

    printf("Opening socket at address %s\n", IP_ADDR);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP_ADDR);
    address.sin_port = 9734;
    len = sizeof(address);
    result = connect(sockfd, (struct sockaddr *)&address, len);

    if (result == -1)
    {
        perror("Client error");
        exit(1);
    }
    printf("Socket connected\n");

get_input:
    // todo: get user input to string
    // todo: close when sending 'q'
    if (strcmp(filename, "q") == 0)
    {
        printf("Closing application\n");
        close(sockfd);
        return 0;
    }

    write(sockfd, &filename, sizeof(filename));
    int filesize = 0;
    read(sockfd, &filesize, sizeof(int));

    if (filesize == 0)
    {
        perror("File wasn't found\n");
        goto get_input;
    }
    printf("Received file of size: %d\n", filesize);
    char file[filesize];
    read(sockfd, &file, filesize);

    goto get_input;
    return -1;
}
