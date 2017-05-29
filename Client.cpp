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
    int host_port = 12400;
    char* host_name = "127.0.0.1" ;  //the ip address for the local host

    //declare structs for socket

    struct clientAddress{
        unsigned short sin_family = AF_INET;    //always AF_INET
        unsigned short sin_port = htons(host_port); //port (16 bits
        struct socketAddress_in sin_addr;   //internet address structure
        char sin_zero[8];   //not used

    };

    struct socketAddress_in{
        unsigned long s_address;   //internet address (32 bits)
    };

    //create structure of type client address called this address
    struct clientAddress thisAddress;

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
        if((err = errno) != WSAEWOULDBLOCK ){
            fprintf(stderr, "Error connecting socket %d\n", errno);
            goto FINISH;
        }
    }

    //now to do actual client stuff


FINISH:
    ;

}

