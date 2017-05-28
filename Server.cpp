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

//the max number of players or max number of pending connections
int MAXPENDING = 10;

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

    socketAddress_in sadr;

    //set the fields for the address
    serverAddress.sin_family = AF_INET; //this is always AF_INET
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);  //directs the server to use any ip address
                                                        // associated with the host
    serverAddress.sin_port = htons(server_port);
    memset(&(serverAddress.sin_zero), 0, 8);

    //bind the port to the socket
    //first, create the error message
    int status = bind(thisSocket, (socketAddress*)&serverAddress, sizeof(serverAddress));

    //if the binding was not successful
    if(status < 0){
        fprintf(stderr, "Error binding to socket. Make sure nothing else is listening"
                "on this port %d\n", errno);
        goto FINISH;
    }

    //set socket to listen
    int listenStatus = listen(thisSocket, MAXPENDING);

    //if the listen is a failure
    if(listen(listenStatus < 0)){
        fprintf(stderr, "Error listening %d\n", errno);
        goto FINISH;
    }

    //accept a connection

    //get the server address size
    address_size = sizeof(socketAddress_in);

    //while no connection is accepted
    while(true){
        //tell the user we're waiting
        printf("Waiting for a connection.\n");

        //create an int to be returned from the attempt at connection
        csock = (int*)malloc(sizeof(int));

        //try and connect
        *csock = accept(thisSocket, (socketAddress*)&sadr, &address_size);

        //if it's successful
        if(*csock != -1){
            printf("-------\nReveived connection from %s\n", inet_ntoa(sadr.sin_addr));

            //create the pthread
            threadErrorCheck = pthread_create(&thread_id, 0, &SocketHandler, (void*)csock);

            //check that thread is created
            if(threadErrorCheck){
                cout <<"Error: unable to create thread " << threadErrorCheck <<endl;
                goto FINISH;
            }
            pthread_detach(thread_id);


        }
    }


}