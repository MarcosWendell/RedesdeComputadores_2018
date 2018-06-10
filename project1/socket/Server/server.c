#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "socketManager.h"
//#define TRUE 1
//#define FALSE 0
#define WINNER_PLAYER1 -1
#define WINNER_PLAYER2 -2
#define DRAW -3

typedef enum{
      FALSE = 0,
      TRUE}
bool;

char *weaponsNames[9] ={"Rock","Paper","Scissors","Lizard","Spock","Spider-Man","Batman","Wizard","Glock"};

int PERDE3[3][3] = {{FALSE, TRUE, FALSE},
                    {FALSE, FALSE, TRUE},
                    {TRUE, FALSE, FALSE}};

int PERDE9[9][9] = { {FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE}, //ROCK
                    {FALSE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE}, //PAPER
                    {TRUE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE}, //SCISSORS
                    {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE}, //LIZARD
                    {FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, TRUE}, //SPOCK
                    {TRUE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE}, //SPIDER-MAN
                    {TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE}, //BATMAN
                    {TRUE, FALSE, TRUE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE}, //WIZARD
                    {FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE}}; //GLOCK

//Mensagens do servidor
char *headline = "================== ROCK PAPER SCISSORS ==================\n" ;
char *newgame = "\t\t\tNEW GAME!\n" ;
char *chooseslevel =
    "Select level:\n"
    "1) Easy (3 elements)\n"
    "2) Advanced (9 elements)\n";
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
    "Enter your weapon:\n"
    "1. Rock\n"
    "2. Paper\n"
    "3. Scissors\n"
    "4. Lizard\n"
    "5. Spock\n"
    "6. Spider-man\n"
    "7. Batman\n"
    "8. Wizard\n"
    "9. Glock\n";
char *playagain =
    "\nPlay Again?\n"
    "Y\n"
    "N\n";
char *valid = "Valid Answer";
char *invalid = "Invalid Answer";

int find_winner(char *choice1, char* choice2, int difficulty){

    int player1, player2;
    player1 = choice1[0] - '0';
    player2 = choice2[0] - '0';
    printf("player1: %d\n", player1);
    if (player1 == player2) return DRAW;
    switch(difficulty){
      case 1:
        if(PERDE3[player1][player2]) return WINNER_PLAYER2;
      break;
      case 2:
        if(PERDE9[player1][player2]) return WINNER_PLAYER2;
      break;
    }
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
//    printf("--open\n");
    read( fd , (*buffer), 256);
//    printf("--close\n");
    printf("%s\n",(*buffer) );

}

bool valid_answer(char *choice, int difficulty){
    if(!strcmp(choice,"1") || !strcmp(choice,"2") || !strcmp(choice,"3")){
      return TRUE;
    }else if(difficulty > 1 && (!strcmp(choice,"4") || !strcmp(choice,"5") || !strcmp(choice,"6") || !strcmp(choice,"7") || !strcmp(choice, "8") || !strcmp(choice, "9"))){
      return TRUE;
    }else{
      return FALSE;
    }
}



int main(int argc, char const *argv[])
{
    int player1, player2; // clientes
    char *buffer;
    char *choice1, *choice2;
    char *weapons;
    int winner;
    int difficulty = 0;
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
    free(buffer);
    buffer = NULL;
    read_message(player2,&buffer);
    free(buffer);
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
//        read_message(player1, &buffer);

        while(difficulty == 0){
//            send_message(player1, "Answer:");
            read_message(player1, &buffer);
            //como so tem o ROCK PAPER SCISSORS basicao tem esse erro
            if(!strcmp(buffer,"1")){
              difficulty = 1;
              weapons = weapon;
              send_message(player1, weapons);
            }else if(!strcmp(buffer,"2")){
              difficulty = 2;
              weapons = weapon2;
              send_message(player1, weapons);
            }else{
              printf("Invalid Answer");
              send_message(player1, invalid);
              free(buffer);
              buffer = NULL;
            }
      }
        /*
        Mensagem enviada para os dois clientes
                    Enter your weapon:
            1. Rock
            2. Paper
            3. Scissors

        */
        send_message(player2,weapons);
while(TRUE){

        //recebe as escolhas dos players
        choice1 = choice2 = NULL;
        while(choice1 == NULL || choice2 == NULL){
          if(choice1 == NULL)read_message(player1,&choice1);
          if(choice2 == NULL)read_message(player2,&choice2);
          if(!valid_answer(choice1,difficulty)){
            send_message(player1,invalid);
            free(choice1);
            choice1 = NULL;
          }
          if(!valid_answer(choice2, difficulty)){
            send_message(player2,invalid);
            free(choice2);
            choice2 = NULL;
          }
        }
        //determina vencedor
        winner = find_winner(choice1,choice2, difficulty);
        if(winner == WINNER_PLAYER1){
            send_message(player1 , won);
            send_message(player2 , lost);
        }else if(winner == WINNER_PLAYER2){
            send_message(player1 , lost);
            send_message(player2 , won);
        }else{
            send_message_all(player1,player2,draw);
        }
        send_message_all(player1,player2,"Other player weapon: ");
        send_message(player1,weaponsNames[atoi(&choice2[0])-1]);
        send_message(player2,weaponsNames[atoi(&choice1[0])-1]);
        send_message_all(player1,player2,playagain);

       /* TENTANTIVA DE CRIAR LOOP FRACASSADA NO MOMENTO (IGNORE)
        send_message_all(player1,player2,playagain);
        read_message(player1,&choice1);
        read_message(player2,&choice2);
        if(choice1[0]=='N' || choice2[0]=='N') break;

     }   */
     choice1 = choice2 = NULL;
     while(choice1 == NULL || choice2 == NULL){
       if(choice1 == NULL)read_message(player1,&choice1);
       if(choice2 == NULL)read_message(player2,&choice2);
       if(strcmp(choice1, "Y") && strcmp(choice1, "N")){
         send_message(player1,invalid);
         free(choice1);
         choice1 = NULL;
       }
       if(strcmp(choice1, "Y") && strcmp(choice1, "N")){
         send_message(player2,invalid);
         free(choice2);
         choice2 = NULL;
       }
     }
     if(!strcmp(choice1,"N") || !strcmp(choice2,"N")){
       send_message_all(player1,player2,"N");
       break;
     }else{
       send_message_all(player1,player2,weapons);
     }
}

    //finalizando jogabilidade
        if(buffer) free(buffer);
        if(choice1) free(choice1);
        if(choice2) free(choice2);

    shutdown(player1,2);
    shutdown(player2,2);

    return 0;
}
