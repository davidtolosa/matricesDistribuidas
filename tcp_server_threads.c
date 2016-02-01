#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include "protocoloMTZ.h"
#include <sqlite3.h>
#include "DBManager.h"
#include "funtionsServer.h"
#include <signal.h>
// COMPILAR: gcc -o ./build/server tcp_server_threads.c protocoloMTZ.c DBManager.c -l pthread -l sqlite3

int signalClose;

void slot_closeMTZ ( int signal)
{
	printf("Se ha precionado Ctrl-c \n");
	signalClose=1;

}

void *cliente (void *);

int main() {

	//Signal para cerrar la app
	//signal(SIGINT, slot_closeMTZ);
	signalClose=0;

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

	initDB(); //Inicio la DB

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

	int threadType=0;

	//suma = (struct psuma *) buffer;

	sdc = *( (int *) arg);

	printf("--------------------------------\n");
	printf("Nuevo Cliente:%i \n",sdc);

	n = 1;
	while ( (n != 0)) {

	protocoloMTZ *mjs;
	mjs = (protocoloMTZ*) malloc(sizeof(protocoloMTZ));

	n = leer_mensaje(sdc, mjs);

	if( n > 0)
		{
			switch (mjs->header.codigo)
			{
				case SOLICITUD_CLIENTE:
				{
					printf("Cliente say: %s\n", mjs->body.mensage);
					printf("--------------------------------\n");

					threadType = SOLICITUD_CLIENTE;

					//Cuando un cliente se conecta.
					newClient(sdc);
					enviar_mensaje(sdc, ACK_CLIENTE_REGISTER, "Hola Cliente. Espero sus actividades\n");

					free(mjs);
					break;
				}
				case SOLICITUD_WORKER:
				{
					printf("Cliente say: %s\n", mjs->body.mensage);
					printf("--------------------------------\n");
					threadType = SOLICITUD_WORKER;

					//Cuando un Worker se conecta
					newWorker(sdc);

					enviar_mensaje(sdc, ACK_WORKER_REGISTER, "Hola Worker\n");

					free(mjs);
					break;
				}
				case OPERACION_SUMA:
					{
						printf("Cliente say: %s\n", mjs->body.mensage);
						printf("--------------------------------\n");


						enviar_mensaje(sdc, ACK_OPERACION, "Operacion recibida aguarde por los resultados.\n");

						// VENDRIA LA LOGICA PARSEAR LAS OPERACIONES
<<<<<<< HEAD

						createOperation(mjs->body.mensage, sdc, OPERACION_SUMA);



=======
							createOperation(mjs->body.mensage, sdc, OPERACION_SUMA);
>>>>>>> 0754d5ec3f02a915338b93ac4a39cb2763e009d4
						//--------------------------------------------------

						free(mjs);
						break;
					}
				case SOLICITUD_TRABAJO:
					{
						printf("Worker dice:%s\n", mjs->body.mensage);

						
						break;
					}
			default:
					{


						break;
					}

			}
		}
		else
		{
			n = 0;
			free(mjs);
			printf("Me despido\n");
			switch (threadType) {
			case SOLICITUD_CLIENTE:
				{

					if(deleteClient(sdc) ==1)
						printf("---Cliente desconectado---\n");
					else
						printf("ERROR BD\n");
					break;
				}
			case SOLICITUD_WORKER:
				{
					deleteWorker(sdc);
					printf("---Worker desconectado---\n");
					break;
				}
			default:
				break;
			}
			//Cuando se desconecta elimino Cliente/worker
			close (sdc);


		}

	}






}
