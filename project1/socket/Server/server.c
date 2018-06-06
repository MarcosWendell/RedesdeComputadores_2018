#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "socketManager.h"
#define TRUE 1
#define FALSE 0
#define WINNER_PLAYER1 0;
#define WINNER_PLAYER2 1;
#define DRAW -1;

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
char *chooseslevel = 
    "================== ROCK PAPER SCISSORS ==================\n" 
    "Select level:\n" 
    "1) Easy (3 elements)\n"
    "2) Medium (5 elements)\n"
    "3) Hard (7 elements)\n"
    "4) Advanced (9 elements)\n";  
char *hello1 = "Hello player 1";
char *hello2 = "Hello player 2";
char *draw = "There is a draw. One way to solve it: Play Again!";
char *won = "Congratulations, you won!";
char *lost = "Sorry, you lost";
char *weapon = 
    "Enter your weapon:"
    "1. Rock"
    "2. Paper"
    "3. Scissors";
char *weapon2 =
    "4. Lizard"
    "5. Spok";
char *weapon3 =
    "6. Spider-man"
    "7. Batman";
char *weapon4 =
    "8. Wizard"
    "9. Glok";


int _winner(int player1, int player2){
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


int main(int argc, char const *argv[])
{
    int player1, player2, valread1, valread2; 
   // int opt = 1;
    char buffer[1024] = {0};
    startingConnection(&player1,&player2);
 
    
    valread1 = read( player1 , buffer, 1024);
    valread2 = read( player2 , buffer, 1024);
    printf("%s\n",buffer );
    send(player1 , hello1 , strlen(hello1) , 0 );
    send(player2 , hello2 , strlen(hello2) , 0 );
    printf("Hello message sent\n");
    return 0;
}