//Author: Muhammad Firdaus Fadlizam
//Date: 10/21/2022
//Program name: Socket Programming (Client)
//Description: This program is based on the socket programming and acts as the client that sends request to the server

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <winsock.h>
#include <Ws2tcpip.h>
#include <string>
#include <sys/types.h>
#include <io.h>
#include <iostream>
#include <conio.h>
using namespace std;

#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996) //disable a warning that prevents code execution
#define DEFAULT_BUFLEN 255 //maximum length size for the message of the day
#define SERVER_PORT 5373 //server port number


int main(int argc, char* argv[])
{
    
    int sockfd, portno = SERVER_PORT, n; //intialize socket, port number & placeholder
    struct sockaddr_in serv_addr;  //declare server address 
    struct hostent* server; 
    char selection[10]; //holds client's request
    char response[8]; //holds server response

    char buffer[DEFAULT_BUFLEN] = ""; //holds message from the server
    int buflen = DEFAULT_BUFLEN;

    WSADATA wsaData; //contains information about windows socket


    int Result = WSAStartup(MAKEWORD(2, 2), &wsaData); //passed the supported version 

    if (Result != NO_ERROR) {
        cout << "WSAStartup failed with error: " << Result;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //declare TCP socket
    serv_addr.sin_family = AF_INET; //TCP
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); //pass the command line argument to the server address variable
    serv_addr.sin_port = htons(portno); //port number

    cout << "-------------------------------------" << endl;
    cout << "     Connecting... " << endl;
    cout << "-------------------------------------" << endl;
    n = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); //connect to the server


    cout << "     Connected to the server" << endl;
    cout << "-------------------------------------" << endl;
    

    char loopCount[2] = "Y"; //loop counter

    //terminate if client requests to close the connection
    while (loopCount[0] != 'N') {
        
        string input;
        cout << "c: ";
        cin >> input; //MSGGET, MSGSTORE or QUIT

        for (int i = 0; i < size(selection); i++) {
            if (i < size(input)) {
                selection[i] = input[i];
            }

            else {
                selection[i] = '\n';
                break;
            }
        }

        n = send(sockfd, selection, 10 , 0); //send client's request to the server
        
        
        //receive the message of the day
        if (input == "MSGGET") {

            cout << "s: ";
            n = recv(sockfd, buffer, buflen, 0);
            printf(buffer);
      
        }

        //replace the message of the day
        else if (input == "MSGSTORE") {

            cout << "s: ";
            n = recv(sockfd, response, 8, 0); //receive server response
            printf(response);

            string strBuffer;
            cout << "c: ";
            cin.ignore();
            getline(cin, strBuffer); //store a string with whitespaces
         
            //move string into the char array before sending to server
            for (int i = 0; i < buflen; i++) {
                if (i < size(strBuffer)) {
                    buffer[i] = strBuffer[i]; 
                }
                else if (isalpha(buffer[i])|| isdigit(buffer[i])) { //delete the previous string in the buffer
                    buffer[i] = '\0';
                }
            }
         
            n = send(sockfd, buffer, (int)strlen(buffer), 0); //send new message of the day to the server
           
            cout << "s: ";
            n = recv(sockfd, response, 8, 0); //print server response
            printf(response);

        }

        //close the connection
        else if (input == "QUIT") {

            cout << "s: ";
            n = recv(sockfd, response, 8, 0); //receive server response
            printf(response);

            loopCount[0] = 'N'; //set loop counter to N to exit

            closesocket(sockfd); //close socket
        }

        cout << endl;
      
    }

    WSACleanup(); //clear WSA
    return 0;
}