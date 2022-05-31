#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "client.h"
#include "../file_handler/file_handler.h"

int client_connect()
{
    return 0;
}

int main()
{
    char IP_ADDR[] = "0.0.0.0";

    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;

    char input[10];
    char *start_msg = "Help:\nTo list files, type 'list'\nTo request a file, type the file name\nTo close the connection, type 'quit'\nInput: ";

    fputs(start_msg, stdout);
    fgets(input, sizeof(input), stdin);

    printf("Opening socket at address %s\n", IP_ADDR);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP_ADDR);
    address.sin_port = htons(9734);
    len = sizeof(address);

    if (connect(sockfd, (struct sockaddr *)&address, len) < 0)
    {
        perror("ERROR connecting");
        exit(1);
    }

    printf("Socket connected\n");

get_input:
    while (1)
    {

        // todo: get user input to string
        // todo: close when sending 'q'
        if (strcmp(input, "quit") == 0)
        {
            printf("Closing application\n");
            close(sockfd);
            return 0;
        }

        int file_str_size = sizeof(input);
        write(sockfd, &file_str_size, sizeof(int));
        write(sockfd, &input, sizeof(input));

        int filesize = 0;
        read(sockfd, &filesize, sizeof(int));
        if (filesize == 0)
        {
            perror("File wasn't found\n");
            goto get_input;
        }
        printf("Received size of %i bytes\n", filesize);
        char file[filesize];
        read(sockfd, &file, filesize);
        save_file(file, "filename");

        // close(sockfd);
        // goto get_input;
    }
    return 0;
}
