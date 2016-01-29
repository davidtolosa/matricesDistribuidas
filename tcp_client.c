#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "protocoloMTZ.h"

//#define P_SIZE sizeof(struct psuma)

int main(int argc, char *argv[]) {
	int n;
	int sd;
	int lon;
	char teclado[512];
	//char buffer[P_SIZE];
	struct sockaddr_in servidor;
	struct sockaddr_in cliente;
	struct hostent *h;
	struct protocoloMTZ *msj;
	char *datos = NULL;

	if (argc < 3) {
		//printf("Debe ejecutar %s (nombre de host)\n",argv[0]);
		printf("Debe ejecutar nombre de host, worker/cliente");
		exit (-1);
	}

	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(4444);
	//servidor.sin_addr.s_addr = inet_addr("x.x.x.x");

	if ( h = gethostbyname ( argv [1] ) ) {
		memcpy(&servidor.sin_addr, h->h_addr, h->h_length);
	}

	lon = sizeof(servidor);

	if (connect (sd, (struct sockaddr *) &servidor, lon) < 0) {

		perror ("Error en connect");
		exit(-1);
	}

	if (strcmp(argv[2],"worker") == 0) {

		printf("Conectado como Worker\n");

		char * buffer = "Hola soy Worker";

		printf("Buffer : %s\n", buffer);

		datos = malloc( sizeof(char) * strlen(buffer));
		memset(datos,0, strlen(buffer));
    sprintf(datos,"%s",buffer);

		enviar_mensaje(sd,SOLICITUD_WORKER,datos);
		//Enviar que es un worker
	}
	else if(strcmp(argv[2],"cliente") == 0){

		printf("Conectado como Cliente\n");

		char * buffer = "Hola soy Cliente";

		printf("Buffer : %s\n", buffer);

		datos = malloc( sizeof(char) * strlen(buffer));
		memset(datos,0, strlen(buffer));
    sprintf(datos,"%s",buffer);

		enviar_mensaje(sd,SOLICITUD_CLIENTE,datos);
		//Enviar que es un cliente
	}
	else{

		printf("Error en el arg[2]\n");

	}

	while (1) {
		
		protocoloMTZ mjs;
		n = leer_mensaje(sd, &mjs);
		
		if( n > 0)
		{
			switch (mjs.header.codigo)
			{
			case ACK_CLIENTE_REGISTER:
				{
					printf("Server say: %s\n", mjs.body.mensage);
					printf("--------------------------------\n");	
				
					break;
				}
			case ACK_WORKER_REGISTER:
				{
					printf("Server say: %s\n", mjs.body.mensage);
					printf("--------------------------------\n");
					
					break;
				}
			default:
				break;
			}
		}
		else
			{
				n = 0;
			}
		
		
		
	}
	printf("El servidor dejo de prestar servicio\n");
	
	close(sd);
}
