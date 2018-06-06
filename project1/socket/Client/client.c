#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "socketManager.h"


int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int client_fd = 0, valread;
    struct sockaddr_in serv_addr;
    
    char *hello = "Player has connected";
    char buffer[1024] = {0};
    client_fd = createSocketFD();

    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    send(client_fd, hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( client_fd , buffer, 1024);
    printf("%s\n",buffer );


    return 0;
}