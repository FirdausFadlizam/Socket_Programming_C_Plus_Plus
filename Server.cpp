//Author: Muhammad Firdaus Fadlizam
//Date: 10/21/2022
//Program name: Socket Programming (Server)
//Description: This program is based on the socket programming and acts as server that will accept a client, and answer the client's requests. 


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <string.h>
#include <io.h>
#include <conio.h>
#include <WinSock2.h>
#include <sys/types.h> 

using namespace std;

#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996) //disable a warning that prevents code execution

#define DEFAULT_BUFLEN 255 //maximum length size for the message of the day
#define SERVER_PORT 5373 //server port number

//display error message
void error(const char* msg)
{
    perror(msg);
    exit(1);
}


int main(int argc, char* argv[]) {

    int sockfd, newsockfd, portno = SERVER_PORT; //intialize socket and port number
    char buffer[DEFAULT_BUFLEN] = "An apple a day keeps the doctor away"; //default message of the day
    int buflen = DEFAULT_BUFLEN; //message size

    char selection[10]; //hold client's request
    char response[8] = "200 OK\n"; //server response

    WSADATA wsaData; //contains information about windows socket

    struct sockaddr_in serv_addr, cli_addr; //declare server and client struct
    int n; //temp


    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); //passed the supported version 

    if (iResult != NO_ERROR) {
        cout << "Error at WSAStartup()\n";

    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
  
    //server address & portNumber
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(portno);

    char server_ip[INET_ADDRSTRLEN] = "";
    inet_ntop(AF_INET, &serv_addr.sin_addr.s_addr, server_ip, INET_ADDRSTRLEN);
  

    //bind the server address to the socket
    if (bind(sockfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    socklen_t length = sizeof(cli_addr);

    //connection request from client
    cout << "-------------------------------------" << endl;
    cout << "  Waiting for client to connect" << endl;
    cout << "-------------------------------------" << endl;
    //allow client to connect 
    listen(sockfd, 20);
    
    //accept client 
    newsockfd = accept(sockfd, (struct sockaddr*) & cli_addr, &length);
    if (newsockfd < 0)
        error("ERROR on accept");

    else {
      
        cout << "  Client is connected to the server" << endl;
     
    }

    char client_ip[INET_ADDRSTRLEN] = "";
    inet_ntop(AF_INET, &cli_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    cout << "  Client IP Address: ";
    printf(client_ip);
    cout << "\n-------------------------------------" << endl;
  
    char loopCount[2] = "Y"; 

    //terminate if client requests to close the connection
    while (loopCount[0] != 'N' && loopCount[0] != 'n') {

        cout << "c: ";
        n = recv(newsockfd, selection, 10, 0); //receive client's request

        for (int i = 0; i < size(selection); i++) {
            if (selection[i] == '\n') {
                selection[i+1] = '\0';
            }
        }
        printf(selection);
     
 
        string option = "";

        //store client's request in a string
        for (int i = 0; i < size(selection); i++) {

            if (selection[i] == '\n')
                break;

            option = option + selection[i];
            
        }
        //client wants to receive the message of the day
        if (option == "MSGGET") {

            string tempMsg = "";

            //transfer response message to tempMsg
            for (int i = 0; i < size(response); i++) {


                if (response[i] == NULL) {
                    break;
                }
                tempMsg = tempMsg + response[i];

            }

            //transfer the message of the day to tempMsg
            for (int i = 0; i < size(buffer); i++) {

                if (buffer[i] == NULL) {
                    tempMsg = tempMsg + '\n';
                    break;
                }
                tempMsg = tempMsg + buffer[i];

            }


            char sendMsg[DEFAULT_BUFLEN] = "";

            //transfer tempMsg into char sendMsg 
            for (int i = 0; i < size(tempMsg); i++) {
                sendMsg[i] = tempMsg[i];
            }
            cout << "s: ";
            n = send(newsockfd, sendMsg, (int)strlen(sendMsg), 0); //send the message of the day
            printf(sendMsg);
         
           
        }

        //clients wants to replace the message of the day
        else if (option == "MSGSTORE") {

            cout << "s: ";
            //send server response to client
            n = send(newsockfd, response, 8, 0);
            
            printf(response);
          
            for (int i = 0; i < buflen; i++) { //clear the message of the day
               
                if (isalpha(buffer[i]) || isdigit(buffer[i])) {
                    
                    buffer[i] = '\0';
                }
               
            }           
           
            cout << "c: "; 
            n = recv(newsockfd, buffer, buflen, 0); //receive client's message of the day
           
            printf(buffer);
            cout << endl;

            cout << "s: ";
            n = send(newsockfd, response, 8, 0); //send server response
            printf(response);
         

        }
        
        //client wants to close the connection
        else if (option == "QUIT") {

            cout << "s: ";
            n = send(newsockfd, response, 8, 0); //send server response
            printf(response);


           

            loopCount[0] = 'N'; //set the loop to N to exit

            closesocket(sockfd);
            closesocket(newsockfd); //close the socket
        }
        cout << endl;
    }
  

    WSACleanup(); //clear WSA
    return 0;
}