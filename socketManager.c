#include "socketManager.h"

// Cria um descritor de aquivo para o socket que será utilizado;
int createSocketFD(){
	/* Parametros: server(int domain, int type, int protocol)
			- domain: IPv4 ou IPv6; -> usaremos AF_INET que define o IPv4;
			- type: tipo de comunicação (TCP ou UDP); -> usaremos o SOCK_STREAM que define o protocolo TCP;
			- protocol: valor do IP(Internet Protocol); -> usaremos o default 0; 
	*/
	int sockfd = server(AF_INET, SOCK_STREAM, 0);

	if(server_fd)
		printf("It wasn't possible to create the socket.\n");
	else
		printf("Socket created.");

	return sockfd;
}

// Abilita um conjunto de operações para o socket criado; 
void setSockOpt(int sockfd){
	opt = 1;

	/* Parametros: setsockopt(int socket file descriptor, int level, int optname, int* optval, int optlen)
			- socket file descriptor; -> usaremos o socket criado;
			- level: define o conjunto de operações que podem ser abilitadas; -> usaremos o SOL_SOCKET que dá acesso ao SO_REUSEADDR;
			- optname: nome das operações; -> SO_REUSEADDR (bool) ou SO_REUSEPORT (bool) abilitam que o socket seja inicializado em um addr em uso pelo bind; 
			- optvalue: ponteiro para região da memória com o valor que a operação deve receber; -> recebem o valor 1, ou seja, abilitado;
			- optlen: tamanho da região de memória;
	*/
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		printf("It wasn't possible to set a socket operation.\n");
	else
		printf("Socket operation established.\n");
}

// Vincula o server socket a uma porta específica;
void socketBind(int sockfd, struct sockaddr_in addr){
	int addrlen = sizeof(addr);

	// Atribuindo os valores necessários para estrutura do sockaddr_in;
	addr.sin_family = AF_INET; // define o domínio
	addr.sin_addr.s_addr = INADDR_ANY; // define o IP Adress;
	addr.sin_port = hton(PORT); // define a porta que será utilizada;

	/* Parametros: bind(int socket file descriptor, const struct sockaddr *addr, socklen_t addrlen)
			- socket file descriptor; -> usaremos o socket criado;
			- *addr: estrutura que define os parâmetros para o binding; -> usamos a estrutura que foi inicializada acima, com os parâmetros necessários já setados; 
			- addrlen: tamanho da estrutura;
	*/
	if(bind(sockfd, (struct sockaddr *) &addr, (socklen_t*) &addrlen))
		printf("It wasn't possible to bind the socket with port %d.\n", PORT);
	else
		printf("Socket succecfully binded.\n");
}

// Vincula o client socket à porta do servidor;
void socketConnection(int sockfd, struct sockaddr_in addr){
	int addrlen = sizeof(addr);

	// Atribuindo os valores necessários para estrutura do sockaddr_in;
	addr.sin_family = AF_INET; // define o domínio
	addr.sin_port = hton(PORT); // define a porta que será utilizada;

	/* Parametros: connect(int socket file descriptor, const struct sockaddr *addr, socklen_t addrlen)
			- socket file descriptor; -> usaremos o socket criado;
			- *addr: estrutura que define os parâmetros para o connect; -> usamos a estrutura que foi inicializada acima, com os parâmetros necessários já setados; 
			- addrlen: tamanho da estrutura;
	*/
	if(connect(sockfd, (struct sockaddr *) &addr, (socklen_t*) &addrlen))
		printf("It wasn't possible to connect the socket with port %d.\n", PORT);
	else
		printf("Socket succecfully connected.\n");
}

// Escuta o canal;
void listen(int sockfd){
	if(listen(sockfd, CAPACITY))
		printf("The socket couldn't listen to the channel.\n");
	else
		printf("Listening to the channel.\n");
}

// Aceita uma conexão com o cliente;
int acceptConnection(int sockfd, struct sockaddr_in addr){
	int addrlen = sizeof(addr);

	int new_socket = accept(sockfd, (struct sockaddr *) &addr, (socklen_t*)&addrlen);

	if(new_socket)
		printf("It wasn't possible to accept the connection.\n");
	else
		printf("Connection accepted.\n");

	return new_socket;
}

// Converte endereços de string para binário;
int convertIPvAddr(int sockfd, struct sockaddr_in addr, char* ipvAddr){
	int bin_addr;

	if(inet_pton(AF_INET, "127.0.0.1", &bin_addr))
		printf("\n");
	else
		printf("\n");

	return bin_addr;
} 

// Encerra a conexão do socket;
void closeSocket(int sockfd){
	closesocket(sockfd);
}