#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include "protocoloMTZ.h"
<<<<<<< HEAD
#include "DBManager.h"

// COMPILAR: gcc -o ./build/server tcp_server_threads.c protocoloMTZ.c DBManager.c -l pthread -l sqlite3
	
=======
#include <sqlite3.h>
#include "DBmanager.h"
>>>>>>> d659bb617cb9f58d11469b6d22618f53df5e6dc5

void *cliente (void *);

int main() {

	int lon;
	int sd;
	int sd_cli;
	struct sockaddr_in sock_servidor;
	struct sockaddr_in sock_cliente;
	pthread_t tid;

	sock_servidor.sin_family = AF_INET;
	sock_servidor.sin_port = htons (4444);
	sock_servidor.sin_addr.s_addr = INADDR_ANY;

	sd = socket (PF_INET, SOCK_STREAM, 0);

	if ( bind ( sd , (struct sockaddr *) &sock_servidor , sizeof(sock_servidor) ) < 0 ) {
		perror("Error en bind");
		exit (-1);
		}

	listen ( sd , 5);

	while (1) {

		lon = sizeof(sock_cliente);

		sd_cli = accept ( sd , (struct sockaddr *) &sock_cliente , &lon);

		pthread_create ( &tid, NULL, cliente, &sd_cli );

	}

	close (sd);

}

void *cliente ( void *arg ) {

	int sdc;
	int n;
	protocoloMTZ mjs;

	//suma = (struct psuma *) buffer;

	sdc = *( (int *) arg);

	printf("--------------------------------\n");
	printf("Nuevo Cliente:%i \n",sdc);


	n = 1;
	while ( n != 0) {

		// aca debemos hacer la logica del cliente (cliente|worker)
		n = leer_mensaje(sdc, &mjs);

		if( n > 0)
		{
			switch (mjs.header.codigo)
			{
				case SOLICITUD_CLIENTE:
				{
					printf("Cliente say: %s\n", mjs.body.mensage);
					printf("--------------------------------\n");
					//Cuando un cliente se conecta.
					
					sqlite3 *handler = ;
					
					
					
					

					break;
				}
				case SOLICITUD_WORKER:
				{
					printf("Cliente say: %s\n", mjs.body.mensage);
					printf("--------------------------------\n");
					//aca debemos poner toda la logica para cuando se conecta el WORKER
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
	printf("Cliente desconectado \n");
	close (sdc);
}
