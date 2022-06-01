#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define N_CLIENTS 10 /* Max number of simultaneous clients */

int server_listen(int *socket);

void *client_handler(void *client_socket);

void increment_client_counter();

void decrement_client_counter();

int main();
