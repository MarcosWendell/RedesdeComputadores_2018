#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "socketManager.h"

char* buffer;


void read_message(int fd){
    buffer = (char*)malloc(sizeof(char)*256);
    //Recebebendo mensagem do servidor
    read( fd , buffer, 256);
    printf("%s\n",buffer );

}




int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int client_fd = 0, valread;
    struct sockaddr_in serv_addr;
    int iamplayer;
  
    char resposta[25];
    char *connecting = "Connecting Player...\nPlayer Connected\n";

    //Fazendo a conexao do cliente com o servidor
    client_fd = createSocketFD();

    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
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

    //A conexao ja esta estabelecida e agora comeca a logica do jogo

    //testendo
    
    send(client_fd, connecting , strlen(connecting) , 0 );
    read_message(client_fd);
    

    //Identifica-se qual player que eh pela mensagem recebida que eh "Xº Player Connected\n" onde X sera o identifier
    if(buffer[0] == '1') iamplayer = 1;
    else if (buffer[0] == '2') iamplayer = 2;
    else return -1;
    buffer = NULL;

    
    if(iamplayer==1){
        printf("Answer:");
        scanf("%s",resposta);
        send(client_fd, resposta, strlen(resposta) , 0 );
      //  read( client_fd , buffer, 1024);

    }
        read_message(client_fd);
        buffer = NULL;

        scanf("%s",resposta);
        send(client_fd, resposta, strlen(resposta) , 0 );
    
        read_message(client_fd);
        buffer = NULL;

        
    
    return 0;
}