#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "client.h"
#include "files.h"

int main()
{
    char IP_ADDR[] = "0.0.0.0";
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    char filename[] = "README.md";

    printf("Opening socket at address %s\n", IP_ADDR);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP_ADDR);
    address.sin_port = htons(9734);
    len = sizeof(address);

    if (connect(sockfd, (struct sockaddr *)&address, len) < 0)
    {
        perror("ERROR connecting to socket");
        exit(1);
    }

    printf("Socket connected\n");

    // todo: get user input to string

    int file_str_size = sizeof(filename);
    write(sockfd, &file_str_size, sizeof(int));
    write(sockfd, &filename, sizeof(filename));

    int filesize = 0;
    read(sockfd, &filesize, sizeof(int));
    if (filesize == 0)
    {
        perror("File doesn't exist\n");
        exit(-1);
    }
    printf("Received size of %i bytes\n", filesize);
    char file[filesize];
    read(sockfd, &file, filesize);
    save_file(file, "filename");

    close(sockfd);
    return 0;
}
