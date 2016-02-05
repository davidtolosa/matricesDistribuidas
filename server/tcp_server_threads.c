#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <sqlite3.h>
#include "../protocol/protocoloMTZ.h"
#include "../db/DBManager.h"
#include "funtionsServer.h"

// COMPILAR: gcc -o ./build/server tcp_server_threads.c protocoloMTZ.c DBManager.c -l pthread -l sqlite3

int signalClose;

void slot_closeMTZ ( int signal)
{
	printf("Se ha precionado Ctrl-c \n");
	signalClose=1;
	exit(signal);
}

void *cliente (void *);
void *showServerInfo(void *arg);

int clients;
int workers;
pthread_mutex_t mClient;
pthread_cond_t cClient;
pthread_mutex_t mWorker;
pthread_cond_t cWorker;

int main(int argc, char *argv[]) {

	//Signal para cerrar la app
	signal(SIGINT, slot_closeMTZ);
	signalClose=0;

	int lon;
	int sd;
	int sd_cli;
	struct sockaddr_in sock_servidor;
	struct sockaddr_in sock_cliente;
	pthread_t tid;
	pthread_t tid_info;

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

	clients=0;
	workers=0;

	pthread_mutex_init(&mClient, NULL);
	pthread_cond_init(&cClient, NULL);
	pthread_mutex_init(&mWorker, NULL);
	pthread_cond_init(&cWorker, NULL);

	pthread_create (&tid_info, NULL, showServerInfo, NULL);

	while (1) {

		lon = sizeof(sock_cliente);

		sd_cli = accept ( sd , (struct sockaddr *) &sock_cliente , &lon);

		pthread_create ( &tid, NULL, cliente, &sd_cli );

	}

	close (sd);

}

void *cliente (void *arg) {

	int sdc;
	int n;

	int threadType=0;
	int id_suboper_worker=0;

	//suma = (struct psuma *) buffer;

	sdc = *( (int *) arg);

	#ifdef DEBUG
	printf("--------------------------------\n");
	printf("Nuevo Cliente:%i \n",sdc);
	#endif

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
					#ifdef DEBUG
					printf("Cliente say: %s\n", mjs->body.mensage);
					printf("--------------------------------\n");
					#endif

					threadType = SOLICITUD_CLIENTE;
					
					pthread_mutex_lock (&mClient);
					clients++;
					pthread_cond_signal (&cClient);
					pthread_mutex_unlock (&mClient);
					
					if(workers>0)
					{
						//Cuando un cliente se conecta.
						newClient(sdc);
						enviar_mensaje(sdc, ACK_CLIENTE_REGISTER, "Hola Cliente. Espero sus actividades\n");


					}
					else
					{
						enviar_mensaje(sdc, NO_WORKER_DISPONIBLES, "Hola Cliente. No hay workers disponibles, intente mas tarde.\n");
					}

					free(mjs);
					break;
				}
				case SOLICITUD_WORKER:
				{
					#ifdef DEBUG
					printf("Cliente say: %s\n", mjs->body.mensage);
					printf("--------------------------------\n");
					#endif

					threadType = SOLICITUD_WORKER;

					//Cuando un Worker se conecta
					newWorker(sdc);

					enviar_mensaje(sdc, ACK_WORKER_REGISTER, "Hola Worker\n");

					pthread_mutex_lock (&mWorker);
					workers++;
					pthread_cond_signal (&cWorker);
					pthread_mutex_unlock (&mWorker);

					free(mjs);
					break;
				}
				case OPERACION_SUMA:
					{
						//El cliente envio las matrices con las que se debe operar
						#ifdef DEBUG
						printf("Cliente say: %s\n", mjs->body.mensage);
						printf("--------------------------------\n");
						#endif

						enviar_mensaje(sdc, ACK_OPERACION, "Operacion recibida aguarde por los resultados.\n");

						// Creo las operaciones a realizar
						createOperation(mjs->body.mensage, sdc, OPERACION_SUMA);

						//--------------------------------------------------
						char* resultado = 0;
						//dejo el thread consultando por los resultados
						while((resultado =checkEndOperation(sdc))==NULL)
							sleep(2);

						#ifdef DEBUG
						printf("\n\n\nRESULTADO: %s\n", resultado);
						#endif
						//cuando los encontro los envio al cliente,
						enviar_mensaje(sdc, RESULTADO_MATRICES, resultado);

						free(resultado);
						free(mjs);
						break;
					}
				case OPERACION_RESTA:
						{
							//El cliente envio las matrices con las que se debe operar
							#ifdef DEBUG
							printf("Cliente say: %s\n", mjs->body.mensage);
							printf("--------------------------------\n");
							#endif

							enviar_mensaje(sdc, ACK_OPERACION, "Operacion recibida aguarde por los resultados.\n");

							// Creo las operaciones a realizar
							createOperation(mjs->body.mensage, sdc, OPERACION_RESTA);

							//--------------------------------------------------
							char* resultado = 0;
							//dejo el thread consultando por los resultados
							while((resultado =checkEndOperation(sdc))==NULL)
								sleep(2);
							//cuando los encontro los envio al cliente,
							enviar_mensaje(sdc, RESULTADO_MATRICES, resultado);

							free(resultado);
							free(mjs);
							break;
						}
				case SOLICITUD_TRABAJO:
					{
						#ifdef DEBUG
						printf("Worker dice:%s\n", mjs->body.mensage);
						#endif

						getSendWork(sdc,&id_suboper_worker);
						sleep(1);


						#ifdef DEBUG
						printf("ID SUBOPERACION WORKER:%i\n",id_suboper_worker);
						#endif
						break;
					}
				case RESULTADO_TRABAJO:
					{
						#ifdef DEBUG
						printf("Worker dice: Resultado del trabajo - %s\n", mjs->body.mensage);
						#endif

						saveResult(mjs->body.mensage,id_suboper_worker);

						//-------------
						//envio el ack para que el worker cambie de estado y vuelta a solicitar trabajos.
						enviar_mensaje(sdc, ACK_OPERACION_WORKER, "Operacion recibida aguarde mas trabajos.\n");

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
			#ifdef DEBUG
			printf("Me despido\n");
			#endif
			switch (threadType) {
			case SOLICITUD_CLIENTE:
				{
					if(deleteClient(sdc) ==1)
						{
							#ifdef DEBUG
							printf("---Cliente desconectado---\n");
							#endif
							pthread_mutex_lock (&mClient);
							clients--;
							pthread_cond_signal (&cClient);
							pthread_mutex_unlock (&mClient);

						}
					else
						{
						#ifdef DEBUG
						printf("ERROR BD\n");
						#endif
						}

					break;
				}
			case SOLICITUD_WORKER:
				{
					if(deleteWorker(sdc)==1)
					{
						#ifdef DEBUG
						printf("---Worker desconectado---\n");
						#endif

						pthread_mutex_lock (&mWorker);
						workers--;
						pthread_cond_signal (&cWorker);
						pthread_mutex_unlock (&mWorker);
						if(workers==0)
							disconnetAllClient();
					}
					{
						#ifdef DEBUG
						printf("ERROR BD\n");
						#endif
					}

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

void *showServerInfo(void *arg){

	#ifdef DEBUG
	printf("CREO EL THREAD INFO");
	#endif
	while (1) {
	system("clear");
	printf("----------------------------------------------------------------------\n");
    printf("-----------------------------Server MTZ-------------------------------\n");
    printf("----------------------------------------------------------------------\n");
    printf("\n");
		//pthread_mutex_lock (&mClient);
		//pthread_cond_wait(&cClient,&mClient);
    printf("Clientes Conectados:%i\n\n",clients);
		//pthread_mutex_unlock (&mClient);
		//pthread_mutex_lock (&mWorker);
    //pthread_cond_wait(&cWorker,&mWorker);
		printf("Workers Conectados:%i\n",workers);
		//pthread_mutex_unlock (&mWorker);

		fflush(stdout);
		sleep(1);
	  }
}
