/* SERVIDOR UDP PARA ANALISE DE TRÁFIGO DE DADOS
 * O SERVIDOR TEM COMO BASE RECEBER UMA MENSAGEM UDP
 * NA MENSAGEM CONTEM O NUMERO DA MENSAGEM E UM TIMESTAMP
 * O SERVIDOR RESPONDE EXATAMENTE A MESMA MENSAGEM PARA O CLIENTE
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//biblioteca de definoções para contar o numero de mensagens recebidas
#include "contamsg.h"

#define Porta 1666

int main()
{
  int server_socket, v6only;
  char buffer[128];
  unsigned int size;

  struct sockaddr_in6 server_addr, client_addr;


  server_socket = socket( AF_INET6, SOCK_DGRAM , 0 );
  //AF_INET6 é IPv6 e SOCK_DGRAM cria socket UDP
    if (server_socket == -1)
    {
      printf("[SERVIDOR] Erro ao criar socket");
      exit(1);
    }
    // Socket funcionando IPv(4)6
    v6only = 0;
  	if (setsockopt(server_socket, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, sizeof(v6only)) != 0)
    {
  		printf("\n[SERVIDOR] Erron no Socket IPv6/IPv4.\n\n");
    	exit(1);
  	}

  	memset(&server_addr, 0, sizeof(server_addr));   // Cleaning the entire struct

 	  server_addr.sin6_family = AF_INET6;           // IPv6
  	server_addr.sin6_addr = in6addr_any;          // Any IP address for the server
  	server_addr.sin6_port = htons(Porta); // Gets the port number by the first argument

  	/* Bind socket with address struct */
 	if(bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
  {
  	printf("\n[SERVIDOR] Erro de Bind\n\n");
  	exit(1);
  }

  size = sizeof(struct sockaddr_in6);


  while (1)
  {
    memset(&buffer, 0, sizeof(buffer));
    recvfrom(server_socket, buffer, 128, 0, (struct sockaddr *) &client_addr, &size); //Recebe a mensagem do cliente

    if (buffer[0] == 'S')
    {
      printf("[SERVIDOR] ultimo pacote recebido!");
      exit(1);
    }
    sendto(server_socket, buffer, 128, 0, (struct sockaddr *) &client_addr, size);
  }


  return 0;
}
