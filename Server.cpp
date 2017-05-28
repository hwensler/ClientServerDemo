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

//the max number of players
int MAX = 10;

//a function to handle the sockets
void* SocketHandler(void*);

int main(int argv, char** argc) {

    //declare the port
    int server_port = 12400;

    //initialize a socket
    int thisSocket;

    //create the socket
    thisSocket = socket(AF_INET, SOCK_STREAM, 0);

    //provide the socket error message
    if(thisSocket < 1){
        printf("There was an error initializing the socket.\n");
        goto FINISH;
    }

    //create structs for server socket information
    struct socketAddress_in serverAddress;

    //set the fields for the address
    serverAddress.sin_family = AF_INET; //this is always AF_INET
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);  //directs the server to use any ip address
                                                        // associated with the host
    serverAddress.sin_port = htons(server_port);
    memset(&(serverAddress.sin_zero), 0, 8);

    //bind the port to the socket
    //first, create the error message
    if(bind(thisSocket, (socketAddress*)&serverAddress, sizeof(serverAddress)) == -1){
        fprintf(stderr, "Error binding to socket. Make sure nothing else is listening"
                "on this port %d\n", errno);
        goto FINISH;
    }

    //set socket to listen
    if(listen(thisSocket, MAX) == -1){
        fprintf(stderr, "Error listening %d\n", errno);
        goto FINISH;
    }


}