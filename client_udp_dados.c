#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>


int main(int argc, char** argv) {

	int client_socket, bytes, rv;
  struct sockaddr_in6;
  char buffer[128];
  fd_set fds;
  struct timeval tv;

  	// Esta é a nova estrutura que vamos utilizar!
	struct addrinfo hints, *list, *item;
	/* Checking the arguments */
  	if(argc != 3) {
    	printf("\n[CLIENTE] Erro de Argumento.\n\n");
    	exit(1);
  	}

  	memset(&hints, 0, sizeof hints); // Enche de zeros na nova estrutura
  	hints.ai_family = AF_UNSPEC;     // aceitar IPv4 ou IPv6
  	hints.ai_socktype = SOCK_DGRAM; // Apenas UDP

  	if((rv = getaddrinfo(argv[1]C, argv[2], &hints, &list)) != 0) { // Chama a função passando os dois argumentos
    	printf("[CLIENTE] Erro de getaddrinfo: %s\n", gai_strerror(rv));
    	exit(1);
  	}


 // ------------------------------------------------------------
 // Para cada item obtido da função....
 	for(item = list; item != NULL; item = item->ai_next) {
    // Tenta criar o socket
    	if((client_socket = socket(item->ai_family, item->ai_socktype, item->ai_protocol)) == -1) {
      		continue;
    	}
		// Se criar o socket, tenta realizar a conexão:
    	if(connect(client_socket, item->ai_addr, item->ai_addrlen) == -1) {
      		close(client_socket); // Não deu certo a conexão! Fecha o socket criado e tenta de novo.
      		printf("[CLIENTE]: Erro de conexao\n");
      		continue;
    	}
    	break;
    }

  if(item == NULL) exit(1); // Caso todos os itens falhem, será o fim da lista.
 // ------------------------------------------------------------
 // A partir deste ponto, estamos conectados!
 // ------------------------------------------------------------

  int f, h;
  int identificadorA;
  char mensagemCliente[200];
  printf("[CLIENTE] Conectado!\n");
  	while(1)
		{
      retorna: //usado no goto;
      FD_ZERO(&fds);//Reseta todos os bits
      FD_SET (client_socket, &fds); //Estabelece o bit que corresponde ao socket clientSocket

      tv.tv_sec = 10; //Estabelece o valor de time out de 7 segundos
      tv.tv_usec = 0; //Estabeele o valor de time de 0 milissegundos

      memset(&mensagemCliente, 0, sizeof(mensagemCliente)); //Zera Memoria da variável
    	memset(&buffer, 0, sizeof(buffer)); //Zera Memoria da variável
    	printf("[CLIENTE] Digite a mensagem Mensagem: ");
    	fgets(buffer, 128, stdin); // Lê a mensagem

      identificadorA = gerarNumeroAleatorio(m); //Gera um valor aleatório para a mensagem, a partir de um parâmetro n;
      m++; // Incrementa o parâmentro;

      bytes = strlen(buffer);
      buffer[--bytes] = '\0';

      sprintf(mensagemCliente, "%s %d", buffer, identificadorA); //Junta a mensagem com o identificador de mensagem gerado aleatoriamente

    	if(buffer[0] == 'q' && buffer[1] == '\0') break; // Teste pra sair com o "q"

      bytes = strlen(mensagemCliente);
      mensagemCliente[bytes+1] = '\0';

      write(client_socket, mensagemCliente, bytes); // Manda a mensagem

      printf("\n[CLIENTE] Mensagem %d Enviada!\n", identificadorA);

    	f = recv(client_socket, buffer, sizeof(buffer), 0);  // Esperando por uma resposta do servidor

      //--------------------------------------------------------------------------------------------------
      // Parte do codigo trata do problema da retransmissao de mensagens
      if(f<0){ //Caso não receba uma resposta
        printf("[CLIENTE] Erro! A mensagem %d sera retransmitida em 7 segundos!\n", identificadorA);

        n = select (client_socket, &fds, NULL, NULL, &tv ); //Inicia temporizador
        if(n==0){ //Caso tenha atingido tempo necessário
          printf("[CLIENTE] Timeout da Mensagem %d. Retransmitindo!\n", identificadorA);
          write(client_socket, mensagemCliente, bytes);
          h = recv(client_socket, buffer, sizeof(buffer), 0);
          if(h<0){ // COnfere se a retransmissao deu ruim
            printf("[CLIENTE] Falha na Retransmissao da mensagem %d! Digite a mensagem novamente!\n", identificadorA);
          }
          else{ // Caso a retransmissao de certo
            goto mensagemRecebida;
          }
        }
        else{
          goto retorna;
        }
      }
      //----------------------------------------------------------------------------------------------------
      else{ // Caso o programa receba a mensagem
        mensagemRecebida:
        printf("\n[CLIENTE] %s\n\n", buffer);
      }
  	}
  	close(client_socket); // Releasing the socket.
  	freeaddrinfo(list); // liberando a memória!!
	return 0;
}
