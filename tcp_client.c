#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include "functionsClient.h"
#include "protocoloMTZ.h"

int signalClose;

void slot_closeMTZ ( int signal)
{
	printf("Se ha precionado Ctrl-c \n");
	signalClose=1;
	exit(signal);
}

int main(int argc, char *argv[]) {

	//Signal para cerrar la app
	signal(SIGINT, slot_closeMTZ);
	signalClose=0;

	int n = 1;
	int sd;
	int lon;

	//char buffer[P_SIZE];
	struct sockaddr_in servidor;
	struct sockaddr_in cliente;
	struct hostent *h;
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

		enviar_mensaje(sd,SOLICITUD_WORKER,"Hola, soy Worker.\n");
		//Enviar que es un worker
	}
	else if(strcmp(argv[2],"cliente") == 0){

		enviar_mensaje(sd,SOLICITUD_CLIENTE,"Hola, soy Cliente.\n");
		//Enviar que es un cliente
	}
	else{

		printf("Error en el arg[2]\n");

	}

	while ((n!=0)) {

		protocoloMTZ *mjs;
		mjs = (protocoloMTZ*) malloc(sizeof(protocoloMTZ));
		n = leer_mensaje(sd, mjs);

		if( n > 0)
		{
			switch (mjs->header.codigo)
			{
				case ACK_CLIENTE_REGISTER:
					{
						printf("Server say: %s\n", mjs->body.mensage);
						printf("--------------------------------\n");

						showHelpClient();

						if(showConsole(sd)<0)
							n=0;

						break;
					}
				case ACK_WORKER_REGISTER:
					{
						printf("Server say: %s\n", mjs->body.mensage);
						printf("--------------------------------\n");

						askForWork(sd);

						break;
					}
				case ACK_OPERACION:
					{
						printf("Server say: %s\n", mjs->body.mensage);
						printf("--------------------------------\n");

						break;
					}
				case ACK_OPERACION_WORKER:
					{
						printf("Server say: %s\n", mjs->body.mensage);
						printf("--------------------------------\n");
						askForWork(sd);

						break;
					}
				case SIN_TRABAJOS:
					{
						printf("Server say: %s\n", mjs->body.mensage);
						printf("--------------------------------\n");

						/*sleep(2);
						askForWork(sd);*/
						break;
					}
				case ASIGNACION_TRABAJO_SUMA:
					{
						printf("Server say: %s\n", mjs->body.mensage);
						printf("--------------------------------\n");

						char* resultado = NULL;
						resultado = solverOperation(mjs->body.mensage, ASIGNACION_TRABAJO_SUMA);

						enviar_mensaje(sd , RESULTADO_TRABAJO, resultado);

						break;
					}
				case ASIGNACION_TRABAJO_RESTA:
					{
						printf("Server say: %s\n", mjs->body.mensage);
						printf("--------------------------------\n");

						char* resultado = NULL;
						resultado = solverOperation(mjs->body.mensage, ASIGNACION_TRABAJO_RESTA);

						enviar_mensaje(sd , RESULTADO_TRABAJO, resultado);

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

		free(mjs);


	}
	printf("Desconectado del servidor\n");

	close(sd);

	return(0);
}
