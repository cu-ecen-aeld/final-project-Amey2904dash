#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define SIZE 80
#define PORT 8080
#define SA struct sockaddr

#define ERROR -1

#define NAMES_FILE_PATH "/etc/face-rec-sample/names.txt"
#define TEMP_FILE_PATH "/etc/face-rec-sample/temp.txt"

 /***********************************************************************************************
* Name          : func
* Description   : function for server- client communication 
* Parameters    : sockfd
* RETURN        : N/A
***********************************************************************************************/
void func(int sockfd)
{
	char name[20];
	int name_fd, temp_fd; 
	int name_ptr = 0;
	char *name_ch=(char*)malloc(sizeof(char));
	char *temp_ch=(char*)malloc(sizeof(char)); 
	
	name_fd = open(NAMES_FILE_PATH, O_RDONLY);
	temp_fd = open(TEMP_FILE_PATH, O_RDONLY);
	//status=read(name_fd, name_ch, 1);
	//printf("Bytes read: %d\n", status);
	while(1)
	{
	    while (read(name_fd, name_ch, 1) == 0);
	    if(1)
	    {
		//printf("name_ch: %c\n", *name_ch);
		    if (*name_ch != '\n')
		    {
			    name[name_ptr++] = *name_ch;
		    }
		    else
		    {
			    name[name_ptr++] = 32;
			    while (read(temp_fd, temp_ch, 1) != 0)
			    {
				//printf("temp_ch: %c\n", *temp_ch);
				    if (*temp_ch != '\n')
				    {
					    name[name_ptr++] = *temp_ch;
				    }
				    else
				    {
					    name[name_ptr++] = '\0';
					    break;
				    }
			    }
			    //printf("string: %s\n", name);
			    if(name != NULL)
			    {
				write(sockfd, name, sizeof(name));
				memset(name, 0, 20);
				name_ptr = 0;
				usleep(1000000);
			    }
		    }
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
    
    if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int))) == -1) {
        printf("socket opt failed...\n");
        exit(0);
    }
    else
        printf("Socket opt succeeded..\n");
        
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

