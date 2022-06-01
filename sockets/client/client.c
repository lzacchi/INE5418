#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "../file_handler/file_handler.h"

#define MAX_C 200 // max size of input string
int client_connect(int *socket, char *input)
{
    int sockfd = *socket;
    int input_size = strlen(input);

    write(sockfd, &input_size, sizeof(int));
    write(sockfd, input, strlen(input));

    int filesize = 0;
    read(sockfd, &filesize, sizeof(int));
    if (filesize == 0)
    {
        perror("File wasn't found\n");
        return 0;
    }
    printf("Downloading file...\n");
    char file[filesize];
    read(sockfd, &file, filesize);
    save_file(file, "client/files/download");
    printf("Saved file to client/files/download\n");

    close(sockfd);
    exit(1);
}

int main()
{
    int sockfd;
    int len;
    struct sockaddr_in address;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9734);
    len = sizeof(address);

    char input[MAX_C];
    char last_char; // last character read in scanf
    int scanf_ret;

    if (connect(sockfd, (struct sockaddr *)&address, len) < 0)
    {
        fprintf(stderr, "Error establishing connection:     %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Socket connected\n\n\n\n");

    char *start_msg = "Help:\nTo request a file, type the file name\nTo close the connection, type 'quit'\nTo see this message again, type 'help'\nInput: ";
    fputs(start_msg, stdout);

    while (strcmp(input, "quit") != 0)
    {
        scanf_ret = scanf(" %199[^\n]%c", input, &last_char);
        if (scanf_ret == EOF)
        { /* check the return against EOF */
            fputs("(user canceled input)\n", stderr);
            return 0;
        }
        else if (scanf_ret < 2)
        { /* checking both string and c read */
            fputs("input failure.\n", stderr);
        }
        else if (last_char != '\n')
        { /* check c is '\n', else string too long */
            fprintf(stderr, "warning: input exceeds %d characters.\n", MAX_C - 1);
            // empty_stdin();
        }
        else if (strcmp(input, "help") == 0)
        {
            fputs(start_msg, stdout);
        }
        else /* good input, output string */
        {
            client_connect(&sockfd, input);
        }
    }

    close(sockfd);
    return 0;
}
