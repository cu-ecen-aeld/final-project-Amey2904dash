#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


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
    int i;
    char buffer[SIZE];
 
    while(1) 
    {
        bzero(buffer, SIZE);
        read(sockfd, buffer, sizeof(buffer)); // read client message and copy that in the buffer
        printf("From client: %s\t To client : ", buffer);
        bzero(buffer, SIZE); // reset string
        i = 0;
        while ((buffer[i++] = getchar()) != '\n'); // wait to copy the server message 
        write(sockfd, buffer, sizeof(buffer)); // send the message to client
        
        if (strncmp("exit", buffer, 4) == 0) // check if 'Exit' is there in message
        {
            printf("Server Exiting...\n");
            break;
        }
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
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
   
    // 1. create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) 
    {
        printf("Error! socket() creation failed\n");
        exit(0);
    }
    else
    {
        printf("socket() creation succeeded \n");
    }
    bzero(&servaddr, sizeof(servaddr)); // reset string
   
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
   
    // 2. Bind the socket
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) 
    {
        printf("Error! bind() socket failed\n");
        exit(0);
    }
    else
    {
        printf("bind() socket succeeded\n");
    }
   
    // 3. Listen on the socket
    if ((listen(sockfd, 5)) != 0) 
    {
        printf("Error! Socket Listen failed\n");
        exit(0);
    }
    else
    {
         printf("Socket Listen succeeded. Server Listening..\n");
    }

    len = sizeof(cli);
   
    //4. Accept the socket  
    connfd = accept(sockfd, (SA*)&cli, (unsigned int *)&len);
    if (connfd < 0) 
    {
        printf("Error: accept() socket failed\n");
        exit(0);
    }
    else
    {
        printf("accept() socket succeeded\n");
    }
   
    //5. Call to function for server- client communication
    func(connfd);
   
    //6. Close the socket
    close(sockfd);
}