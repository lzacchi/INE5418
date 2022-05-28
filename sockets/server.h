#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define N_CLIENTS 2 /* Max number of simultaneous clients */

int server_listen(int *socket);

void *client_thread(void *args);

int main();
