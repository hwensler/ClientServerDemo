/**
 * The client side of a client-server guessing game.
 * Author: Heather Wensler
 */

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#ifdef __WIN32__
# include <winsock2.h>
#else
# include <sys/socket.h>
#include <netinet/in.h>
#include <resolv.h>
#include <arpa/inet.h>
#endif


int main(int argv, char** argc) {
    //declare port information
    int host_port = 12401;
    char* host_name = "127.0.0.1" ;  //the ip address for the local host

    //declare structs for sockets
    struct sockaddr_in thisAddress;
    thisAddress.sin_family =  AF_INET;    //always AF_INET
    thisAddress.sin_port = htons(host_port); //port (16 bits)
    memset(&(thisAddress.sin_zero), 0, 8);
    thisAddress.sin_addr.s_addr = inet_addr(host_name);

    int thisSocket; //an int for this socket
    int err;    //a variable for errors

    //create the socket
    thisSocket = socket(AF_INET, SOCK_STREAM, 0);

    //error message for if the socket isn't created correctly
    if (thisSocket < 0){
        printf("Error initializing socket %d\n", errno);
    }

    //establish connection
    int connection = connect(thisSocket, (struct sockaddr*)&thisAddress, sizeof(thisAddress));

    //create error information
    if(connection == -1){
        //WSAEWOULDBLOCK  in windows, EINPROGRESS in Linux
        if((err = errno) !=  EINPROGRESS ){
            fprintf(stderr, "Error connecting socket %d\n", errno);
            goto FINISH;
        }
    }

    //now to do actual client stuff
    printf("Welcome to the number guessing game!\nEnter your name:\n");

    //remember - you can't restrict the length of the name
    char buffer[1024];
    int buffer_length = 1024;
    int bytecount;  //used to return from send

    //get the name
    fgets(buffer, 1024, stdin);
    buffer[strlen(buffer)-1] = '\0';

    //send data

    //if it doesn't send
    if((bytecount=send(thisSocket, buffer, strlen(buffer), 0)) == -1){
        fprintf(stderr, "Error sending data %d\n", errno);
        goto FINISH;
    }
    //confirm the name was sent
    printf("Your name has been recorded.\n Now, it's time to set a guessing game record!\n");


FINISH:
    ;

}

