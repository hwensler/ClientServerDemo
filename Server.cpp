/**
 * The server side of a client-server number guessing game.
 * Author: Heather Wensler
 */
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <string>
#ifdef __WIN32__
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

//the max number of players or max number of pending connections
int MAXPENDING = 10;
pthread_t thread_id;


//a function to handle the sockets
void* SocketHandler(void*);

using namespace std;

int main(int argv, char** argc) {

    //define structs for address stuff
    struct sockaddr_in socketAddress_in;
    struct sockaddr_in thisAddress;

    //get the server address size
    socklen_t address_size = 0;
    sockaddr_in sadr;

    //create status for error messages
    int status;

    //initialize csock
    int* csock;


    //declare the port
    int server_port = 12402;

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

    thisAddress.sin_family = AF_INET;    //always AF_INET
    thisAddress.sin_port = htons(server_port);
    memset(&(thisAddress.sin_zero), 0, 8);
    thisAddress.sin_addr.s_addr = htonl(INADDR_ANY); //directs the server to use any ip address


    //bind the port to the socket
    //first, create the error message
    status = bind(thisSocket, (sockaddr*)&thisAddress, sizeof(thisAddress));

    //if the binding was not successful
    if(status < 0){
        fprintf(stderr, "Error binding to socket. Make sure nothing else is listening"
                " on this port %d\n", errno);
        goto FINISH;
    }


    //set socket to listen
    //if the listen is a failure
    if(listen(thisSocket, MAXPENDING) == -1){
        fprintf(stderr, "Error listening %d\n", errno);
        goto FINISH;
    }

    //accept a connection

    //while no connection is accepted
    address_size = sizeof(sockaddr_in);


    while(true){
        //tell the user we're waiting
        printf("Waiting for a connection.\n");

        //create an int to be returned from the attempt at connection
        csock = (int*)malloc(sizeof(int));

        //try and connect
        //initialize client socket
        *csock = accept(thisSocket, (sockaddr*)&sadr, &address_size);

        //if it's successful
        if(*csock != -1){
            printf("-------\nReveived connection from %s\n", inet_ntoa(sadr.sin_addr));

            //create the pthread
           int  threadErrorCheck = pthread_create(&thread_id, 0, &SocketHandler, (void*)csock);

            //check that thread is created
            if(threadErrorCheck){
                cout <<"Error: unable to create thread " << threadErrorCheck <<endl;
                goto FINISH;
            }
            pthread_detach(thread_id);
        }
        //if it's not successful
        else{
            fprintf(stderr, "Error accepting %d\n", errno);
        }
    }
    FINISH:
    ;
}

void* SocketHandler(void* lp){
    int *csock = (int*)lp;

    char nameBuffer[1024];
    int buffer_len = 1024;
    int bytecount;

    memset(nameBuffer, 0, buffer_len);

    //set the number the client must guess
    int theNumber = rand()%(9999);

    //print out for the grader/debugging
    cout << "The number you're guessing is " << theNumber << ".\n";

    if((bytecount = recv(*csock, nameBuffer, buffer_len, 0)) == -1){
        fprintf(stderr, "Error receiving name %d\n", errno);
        goto FINISH;
    }

    cout << "Alright, " << nameBuffer << "! Let's see what you can do!\n";

    FINISH:
        free(csock);
        return 0;
}