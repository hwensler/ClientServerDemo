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

int main(int argv, char** argc) {

    //declare the port
    int host_port = 12400;

    //initialize a socket
    int thisSocket;

    //create the socket
    thisSocket = socket(AF_INET, SOCK_STREAM, 0);

    //provide the socket error message
    if(thisSocke < 1){
        printf("There was an error initializing the socket.\n");
        goto FINISH;
    }
}