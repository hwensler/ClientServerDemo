/**
 * The server side of a client-server number guessing game.
 * Author: Heather Wensler
 */
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

//a function to handle the sockets
void* SocketHandler(void*);