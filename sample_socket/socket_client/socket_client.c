/*
 * @file_name	: socket_client.c
 *
 * @author	: Amey Dashaputre, Tanmay Kothale, Varun Mehta
 *
 * @references	: https://github.com/cu-ecen-aeld/assignments-3-and-later-Amey2904dash/blob/main/server/aesdsocket.c
 *
 * @brief	: Socket client application code. Connects to server, reads data sent from server, and prints it on 
 *		  16x4 LCD Display.
 *
 */

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "lcd.h"
#include "wiringPi.h"

#define SIZE 80
#define PORT 8080
#define SA struct sockaddr

/***********************************************************************************************
* Name          : func
* Description   : function for server- client communication 
* Parameters    : sockfd
* RETURN        : N/A
***********************************************************************************************/
void func(int sockfd)
{
    char buffer[SIZE]; 	//recieved data from server
    char buffer2[SIZE];	//modified data to print on LCD
    int flag = 0;		//sets when name is stored successfully
    int j = 0, i = 0;; 	//loop variables
  
    while(1) 
    {
        //clear the buffer
        bzero(buffer, sizeof(buffer));
        bzero(buffer2, sizeof(buffer2));
        
        //wait here till there is data to be read from server
        //read client message and copy that in the buffer
        while(read(sockfd, buffer, sizeof(buffer))==0); 
        
        i=-1; j =0; flag = 0; //reset all loop variables and flags
        while (++i<20)
        {   
            if (buffer[i] != '-' && flag != 1)	//check whether name is read from packet successfully
            {
                buffer2[j++] = buffer[i];
            }
            else
            {
                
                flag = 1;				//once name is read, add extra spaces to isolate temperature
                while (j < 16)			//value from data packet
                {
                    buffer[i] = ' ';
                    buffer2[j++] = ' ';
                }
                buffer2[j++] = buffer[i];
            }
        }
        
        //display it on LCD
        lcd_clear();
        delay(1000);
        lcd_print(buffer2);
        
        //modified data packet printed on LCD (debug)
        //printf("For LCD : %s\n\r", buffer2);
        
        //print the message received from server (debug)
	//printf("From Server : %s\n\r", buffer);

    }
}

/***********************************************************************************************
* Name          : Main
* Description   : Entry point function
* Parameters    : N/A
* RETURN        : N/A
***********************************************************************************************/   
int main(int argc, char** argv[])
{
    
    if (argc < 2)
    {
    	printf("Please provide ip address\n");
    	exit(EXIT_FAILURE);
    }
    
    lcd_init(); //initialize LCD display
    SetChrMode(); 
    
    int sockfd;
    struct sockaddr_in servaddr;
   
    // 1. create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else

    bzero(&servaddr, sizeof(servaddr));  // reset string
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(PORT);
   
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)      // connect the client socket to server socket
    { 
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
   
    func(sockfd);  
   
    close(sockfd);    // close the socket
}

