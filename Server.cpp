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
#include <ctime>
#include <mutex>
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

//a function that calculates the result of a guess
int Distance(int guess, int number){
    int runningTotal = 0;
    int remainingGuess = guess;
    int remainingNumber = number;
    while(remainingGuess > 0 || remainingNumber > 0){
        runningTotal = runningTotal + abs((remainingGuess) % 10 - (remainingNumber % 10));
        remainingGuess = remainingGuess/10;
        remainingNumber = remainingNumber/10;
    }
    return runningTotal;
}

using namespace std;

int main(int argv, char** argc) {

    //using a random seed based on time
    srand(time(NULL));

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

    //declare server port
    int server_port = atoi(argc[1]);

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

    //create buffers for the game communication
    char guessBuffer[1024];
    int guessBuffer_len = 1024;
    int guessBytecount;
    char resultBuffer[1024];

    //set an int for the guess
    int guess;

    //creates a count to tell how many turns it's taken
    int count = 0;
    char countString[16384] ;

    int *csock = (int*)lp;

    //create buffer info for communication
    char nameBuffer[1024];
    int nameBuffer_len = 1024;
    int nameBytecount;

    //prepare victory messages
    string victoryString;
    char victoryMessage[1024];

    memset(nameBuffer, 0, nameBuffer_len);
    memset(victoryMessage, 0, 16384);

    //set leaderboard
    string leaderboardNames[3] = {" ", " ", " "};
    int leaderboardScores[3] = {99999, 99999, 99999};
    char score1[1024];
    char score2[1024];
    char score3[1024];

    //create temps for leaderboard update
    string tempNewName;
    int tempNewScore = count;
    string tempOldName;
    int tempOldScore;


    //set mutex for leaderboard access
    pthread_mutex_t leaderboardMutex = PTHREAD_MUTEX_INITIALIZER;

    //set the number the client must guess
    int theNumber = rand()%(9999);

    //print out for the grader/debugging
    cout << "The number you're guessing is " << theNumber << ".\n";

    if((nameBytecount = recv(*csock, nameBuffer, nameBuffer_len, 0)) == -1){
        fprintf(stderr, "Error receiving name %d\n", errno);
        goto FINISH;
    }

    //show you've received the name
    cout << "Alright, " << nameBuffer << "! Let's see what you can do!\n";

    guess = atoi(guessBuffer);

    //set tempNewName
    tempNewName = (string)nameBuffer;

    //while they haven't guessed the number
    while(guess != theNumber){

        //increment count
        count++;

        //get the number
        if((guessBytecount = recv(*csock, guessBuffer, guessBuffer_len, 0)) == -1){
            fprintf(stderr, "Error receiving guess %d\n", errno);
            goto FINISH;
        }

        //set the guess
        guess = atoi(guessBuffer);

        //calculate the result
        int result = Distance(guess, theNumber);

        //convert it to a string
        sprintf(resultBuffer, "%d", result);
        cout << resultBuffer;

        //send the result
        if((guessBytecount=send(*csock, resultBuffer, strlen(guessBuffer), 0)) == -1){
            fprintf(stderr, "Error sending data %d\n", errno);
            goto FINISH;
        }

    }


    //access the leaderboard
    pthread_mutex_lock(&leaderboardMutex);
    for(int i = 0; i < 3; i++){
        if(tempNewScore < leaderboardScores[i]){
            tempOldName = leaderboardNames[i];
            tempOldScore = leaderboardScores[i];
            leaderboardNames[i] = tempNewName;
            leaderboardScores[i] = tempNewScore;

            tempNewName = tempOldName;
            tempNewScore = tempOldScore;
        }
    }
    pthread_mutex_unlock(&leaderboardMutex);

    sprintf(countString, "%d", count);

    //convert scores to strings
    sprintf(score1, "%d", leaderboardScores[0]);
    sprintf(score2, "%d", leaderboardScores[1]);
    sprintf(score3, "%d", leaderboardScores[2]);
    //send a victory message
    victoryString = "Congratulations! You won the game in " + (string)countString + " turns.\n\n" +
            "Leaderboard: " + "1. " + leaderboardNames[0] +" " + (string)score1+ " \n" +
            "2. " + leaderboardNames[1] + " " + (string)score2 + " \n" +
            "3. " + leaderboardNames[2] + " " + (string)score3 + " \n";

    //convert victory string to char array
    cout << "Victory message to send: " << victoryString << "\n";
    copy(victoryString.begin(),victoryString.end(),victoryMessage);


    if((guessBytecount=send(*csock, victoryMessage, 16384, 0)) == -1){
        fprintf(stderr, "Error sending data %d\n", errno);
        goto FINISH;
    }



    FINISH:
        free(csock);
        return 0;
}