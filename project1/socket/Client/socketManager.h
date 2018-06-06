#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 8080
#define CAPACITY 3 // capacidade da pilha de conexões pendentes;

// Cria um descritor de aquivo para o socket que será utilizado;
int createSocketFD();

// Abilita um conjunto de operações para o socket criado; 
void setSockOpt(int);

// Vincula o server socket a uma porta específica;
void socketBind(int, struct sockaddr_in);

// Vincula o client socket à porta do servidor;
void socketConnection(int, struct sockaddr_in);

// Escuta o canal;
void listenSocket(int);

// Aceita uma conexão com o cliente;
int acceptConnection(int, struct sockaddr_in);

// Converte endereços de string para binário;
int convertIPvAddr(int, struct sockaddr_in, char*);

// Encerra a conexão do socket;
void closeSocket(int);
#endif