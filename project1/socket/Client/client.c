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
    int server_fd = 0;
    struct sockaddr_in serv_addr;
    int iamplayer; //ID do cliente
  
    char resposta[25];
    char *connecting = "Connecting Player...\nPlayer Connected\n";

    //Fazendo a conexao do cliente com o servidor
    server_fd = createSocketFD();

    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    //A conexao ja esta estabelecida e agora comeca a logica do jogo


    send(server_fd, connecting , strlen(connecting) , 0 );
    
    /*
    Mensagem recebida do servidor:
    "Xº Player Connected\n" sendo X o valor 1 ou 2 dependendo da ordem de conexao
    */
    read_message(server_fd);
    

    //Identifica-se qual player que eh pela mensagem recebida que eh "Xº Player Connected\n" onde X sera o identifier
    if(buffer[0] == '1') iamplayer = 1;
    else if (buffer[0] == '2') iamplayer = 2;
    else return -1;
    buffer = NULL;

        
        if(iamplayer==1){ //caso seja o primeiro a conectar escolhe a dificuldade do jogo
            printf("Answer:");
            scanf("%s",resposta);
            send(server_fd, resposta, strlen(resposta) , 0 );
        }
        /*
        Mensagem recebida por player 2:
            Wait Player 1 Select Level...
        Mensagem recebida pelos 2 players:
            Enter your weapon:
            1. Rock
            2. Paper
            3. Scissors
        */
            read_message(server_fd);
            buffer = NULL;
        //cada cliente envia sua resposta
            scanf("%s",resposta);
            send(server_fd, resposta, strlen(resposta) , 0 );
        //cliente recebe a mensagem se ganhou, perdeu ou deu empate
            read_message(server_fd);
            buffer = NULL;
        //pergunta-se se o cliente que jogar de novo    
            scanf("%s",resposta);
            send(server_fd, resposta, strlen(resposta) , 0 );
           // if(resposta[0] = 'N') break;

    

        if(buffer) free(buffer);
        shutdown(server_fd,2);
    
    return 0;
}