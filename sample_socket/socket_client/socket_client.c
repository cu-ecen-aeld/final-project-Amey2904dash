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
    char buffer[SIZE];
    char buffer2[SIZE];
    //int count = 0;
    int flag = 0;
    int j = 0; 
    int i=0;
    while(1) 
    {
        //clear the buffer
        bzero(buffer, sizeof(buffer));
        bzero(buffer2, sizeof(buffer2));
        
        //wait here till there is data to be read from server
        //read client message and copy that in the buffer
        while(read(sockfd, buffer, sizeof(buffer))==0); 
        
        i=-1; j =0; flag = 0;
        while (++i<20)
        {   
            if (buffer[i] != '-' && flag != 1)
            {
                buffer2[j++] = buffer[i];
            }
            else
            {
                
                flag = 1;
                while (j < 16)
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
        
        //printf("For LCD : %s\n\r", buffer2);
        
        //print the message received from server
        printf("From Server : %s\n\r", buffer);

    }
}

/***********************************************************************************************
* Name          : Main
* Description   : Entry point function
* Parameters    : N/A
* RETURN        : N/A
***********************************************************************************************/   
int main()
{
    
    lcd_init();
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
    servaddr.sin_addr.s_addr = inet_addr("10.0.0.85");
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

