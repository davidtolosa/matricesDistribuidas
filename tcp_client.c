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
	char teclado[256];
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
						while(n!=0)
						{
							
							
							printf( "$ >");
							fgets(teclado, sizeof(teclado), stdin);
							teclado[strlen(teclado) - 1] = '\0';
							fflush(stdin);
							
							char* operacion = strtok (teclado," ");
							
							if( strcmp(operacion, "SUMA")==0)
								{
										/*Obtengo las cadenas para poder procesarlas	*/
										char *file1 = strtok (NULL," ");
										char *file2 = strtok (NULL," ");
										printf( "FILE 1 : %s\n", file1 );
										printf( "FILE 2 : %s\n", file2 );
								}
							else 
								{
								if( strcmp(operacion, "RESTA")==0)
									{
										/*Obtengo las cadenas para poder procesarlas	*/
										char *file1 = strtok (NULL," ");
										char *file2 = strtok (NULL," ");
										printf( "FILE 1 : %s\n", file1 );
										printf( "FILE 2 : %s\n", file2 );
									}
								else if( strcmp(operacion, "SALIR")==0)
									{
										/*Verifico que no sea un operacion invalida	*/
										n=0;
									}
									else if( strcmp(operacion, "HELP")==0)
									{
										system("clear");
										showHelpClient();
									}
									else 
										{ 
												printf("Operacion Invalida\n");
										}
							
								}
						
							
						}	
						
						break;
					}
				case ACK_WORKER_REGISTER:
					{
						printf("Server say: %s\n", mjs->body.mensage);
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
		
		free(mjs);


	}
	printf("Desconectado del servidor\n");

	close(sd);
	
	return(0);
}
