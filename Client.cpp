/**
 * The client side of a client-server guessing game.
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
#ifdef __WIN32__
# include <winsock2.h>
#else
# include <sys/socket.h>
#endif

using namespace std;

int main(int argv, char** argc) {
    //declare port information
    int host_port = 12400;
    char* host_name = "127.0.0.1" ;  //the ip address for the local host

    //declare structs for socket
    struct socketAddress_in;
    struct clientAddress;

    int thisSocket; //an int for this socket
    int err;    //a variable for errors

    //create the socket
    thisSocket = socket(AF_INET, SOCK_STREAM, 0);

    //error messge for if the socket isn't created correctly
    if (thisSocket < 0){
        printf("Error initializing socket %d\n", errno);
    }

}

