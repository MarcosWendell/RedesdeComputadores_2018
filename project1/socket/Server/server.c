#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "socketManager.h"
#define TRUE 1
#define FALSE 0
#define WINNER_PLAYER1 -1
#define WINNER_PLAYER2 -2
#define DRAW -3


int PERDE[9][9] = { {FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE}, //ROCK
                    {FALSE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE}, //PAPER
                    {TRUE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE}, //SCISSORS
                    {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE}, //LIZARD
                    {FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, TRUE}, //SPOK
                    {TRUE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE}, //SPIDER-MAN
                    {TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE}, //BATMAN
                    {TRUE, FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE}, //WIZARD
                    {FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE}}; //GLOK

//Mensagens do servidor
char *headline = "================== ROCK PAPER SCISSORS ==================\n" ;   
char *newgame = "\t\t\tNEW GAME!\n" ;             
char *chooseslevel = 
    "Select level:\n" 
    "1) Easy (3 elements)\n"
    "2) Medium (5 elements) => Not working\n"
    "3) Hard (7 elements) => Not working\n"
    "4) Advanced (9 elements) => Not working\n";  
char *waitplayer = "Wait Player 1 Select Level..." ;   
char *hello1 = "1º Player Connected\n";
char *hello2 = "2º Player Connected\n";
char *draw = "There is a draw. One way to solve it: Play Again!\n";
char *won = "Congratulations, you won!\n";
char *lost = "Sorry, you lost\n";
char *weapon = 
    "Enter your weapon:\n"
    "1. Rock\n"
    "2. Paper\n"
    "3. Scissors\n";
char *weapon2 =
    "4. Lizard\n"
    "5. Spok\n";
char *weapon3 =
    "6. Spider-man\n"
    "7. Batman\n";
char *weapon4 =
    "8. Wizard\n"
    "9. Glok\n";
char *playagain =
    "Play Again?\n"
    "Y\n"
    "N\n";

int find_winner(char *choice1, char* choice2){

    int player1, player2;
    player1 = choice1[0] - '0';
    player2 = choice2[0] - '0';
    printf("player1: %d\n", player1);
    if (player1 == player2) return DRAW;
    if(PERDE[player1][player2]) return WINNER_PLAYER2;
    return WINNER_PLAYER1;
}

void startingConnection(int* player1,int *player2 ){
  int server_fd;
  struct sockaddr_in address;
 // int addrlen = sizeof(address);
     
     server_fd = createSocketFD();
      
    // Forcefully attaching socket to the port 8080
  setSockOpt(server_fd);

  socketConnection(server_fd, address); 
  socketBind(server_fd, address);
  


   listenSocket(server_fd);
    int p1 = acceptConnection(server_fd, address);
    int p2 = acceptConnection(server_fd, address);

    (*player1) = p1;
    (*player2) = p2;

}
void send_message_all(int player1, int player2 ,char *s){
    send(player1 , s , strlen(s) , 0 );
    send(player2 , s , strlen(s) , 0 ); 
}
void send_message(int player, char *s){
    send(player , s , strlen(s) , 0 );
}

void read_message(int fd, char **buffer){
    (*buffer) = (char*)malloc(sizeof(char)*50);
    //Recebebendo mensagem do servidor
    read( fd , (*buffer), 256);
    printf("%s\n",(*buffer) );

}




int main(int argc, char const *argv[])
{
    int player1, player2; // clientes
    char *buffer; 
    char *choice1, *choice2;
    int winner;
    //conectando servidor com seu clientes
    //Precisa ter 2 clientes senao fica em estado de espera
    startingConnection(&player1,&player2);
 
    /* 
    Os 2 clientes enviam a seguinte mensagem para o servidor mostrando que estao conectador
    Connecting Player...
    Player Connected
    Obs: nao funciona se o servidor nao receber alguma mensagem de confirmacao
    */
    read_message(player1,&buffer);
    buffer = NULL;
    read_message(player2,&buffer);
    buffer = NULL;

    /*
    Server envia mensagem "Xº Player Connected\n" sendo X o valor 1 ou 2 dependendo da ordem de conexao
    Isso ajuda os clientes a se identificarem no client.c
    */
    send(player1 , hello1 , strlen(hello1) , 0 );
    send(player2 , hello2 , strlen(hello2) , 0 );
    
    //while(1){

        /*
        Mensagem enviada para os dois clientes 
        ================== ROCK PAPER SCISSORS ==================
                                NEW GAME!
        */
        send_message_all(player1, player2,headline);
        send_message_all(player1, player2,newgame);
        
        /*
        Apenas o player 1 escolhe o nivel do jogo
        Para o player 1 aparece:
                        Select level:
                1) Easy (3 elements)
                2) Medium (5 elements) => Not working
                3) Hard (7 elements) => Not working
                4) Advanced (9 elements) => Not working
        Para o player 2 aparece:
            Wait Player 1 Select Level...
        */
        send_message(player1 , chooseslevel);
        send_message(player2 , waitplayer);
        //recebe a escolha feita pelo player1
        read_message(player1, &buffer);

      
        //como so tem o ROCK PAPER SCISSORS basicao tem esse erro
        if(buffer[0] != '1'){ 
            printf("Invalid Answer\n");
            return -1;
        }
       
        /*
        Mensagem enviada para os dois clientes 
                    Enter your weapon:
            1. Rock
            2. Paper
            3. Scissors

        */
        send_message_all(player1,player2,weapon);
       
        //recebe as escolhas dos players
        read_message(player1,&choice1);
        read_message(player2,&choice2);
       
        //determina vencedor
        winner = find_winner(choice1,choice2);
        if(winner == WINNER_PLAYER1){
            send_message(player1 , won);
            send_message(player2 , lost);
        }else if(winner == WINNER_PLAYER2){
            send_message(player1 , lost);
            send_message(player2 , won);
        }else{
            send_message_all(player1,player2,draw);
        }
    
       /* TENTANTIVA DE CRIAR LOOP FRACASSADA NO MOMENTO (IGNORE)
        send_message_all(player1,player2,playagain);
        read_message(player1,&choice1);
        read_message(player2,&choice2);
        if(choice1[0]=='N' || choice2[0]=='N') break;       

     }   */


    //finalizando jogabilidade
        if(buffer) free(buffer);
        if(choice1) free(choice1);
        if(choice2) free(choice2);

    shutdown(player1,2);
    shutdown(player2,2);

    return 0;
}