/**
 * The client side of a client-server guessing game.
 * Author: Heather Wensler
 */

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <string>
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

using namespace std;

int main(int argv, char** argc) {
    //declare port information
    int host_port = atoi(argc[1]);
    char* host_name = "127.0.0.1" ;  //the ip address for the local host

    int guess;  //the guess you've made

    //initialize a counter for the count you're on
    int count = 0;

    //initialize the result to something that's not 0
    int result = 1;

    //declare structs for sockets
    struct sockaddr_in thisAddress;
    thisAddress.sin_family =  AF_INET;    //always AF_INET
    thisAddress.sin_port = htons(host_port); //port (16 bits)
    memset(&(thisAddress.sin_zero), 0, 8);
    thisAddress.sin_addr.s_addr = inet_addr(host_name);

    int thisSocket; //an int for this socket
    int err;    //a variable for errors

    //these are the values for communication
    char buffer[1024];
    int buffer_length = 1024;
    int bytecount;  //used to return from send

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

    //while you haven't guessed the number
    while(result != 0){

        //increment count
        count++;
        cout << "Turn: " << count <<"\n";

        while(true){
            //get the guess
            printf("Enter a guess: \n");
            fgets(buffer, 1024, stdin);

            //cast buffer as int to store guess
            guess = atoi(buffer);

            //make sure the guess is valid
            //if the person guessed 0 OR if the guess is in the right range
            if((guess == 0 && buffer == "0")|| (guess >= 0 && guess < 10000)){
                break;
            }
            else{
                printf("Please pick an integer between 0 and 9999. \n");
            }
        }

        //if the input guess is good, send it to the server
        if((bytecount=send(thisSocket, buffer, strlen(buffer), 0)) == -1){
            fprintf(stderr, "Error sending data %d\n", errno);
            goto FINISH;
        }

        //now receive the result of your guess
        if((bytecount = recv(thisSocket, buffer, buffer_length, 0))== -1){
            fprintf(stderr, "Error receiving data %d\n", errno);
            goto FINISH;
        }

        //record the result
        result = atoi(buffer);

        //print the result
        cout << "Result of the guess: " << result << ".\n";
    }

    //tell them they won the game
    cout << "Congratulations! It took " << count << " turns to guess the number! ";



FINISH:
    ;

}

