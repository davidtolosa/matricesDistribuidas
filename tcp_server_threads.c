#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

// COMPILAR: cc -o ser tcp_server_threads.c -l pthread

void *cliente (void *);

struct psuma {
	uint16_t v1;
	uint16_t v2;
	uint32_t res;
};

int main() {

	int lon;
	int sd;
	int sd_cli;
	struct sockaddr_in servidor;
	struct sockaddr_in cliente;
	pthread_t tid;
	
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons (4444);
	servidor.sin_addr.s_addr = INADDR_ANY;

	sd = socket (PF_INET, SOCK_STREAM, 0);

	if ( bind ( sd , (struct sockaddr *) &servidor , sizeof(servidor) ) < 0 ) {

		perror("Error en bind");
		exit (-1);

	}

	listen ( sd , 5);

	while (1) {

		lon = sizeof(cliente);

		sd_cli = accept ( sd , (struct sockaddr *) &cliente , &lon);

		pthread_create ( &tid, NULL, cliente, &sd_cli );

	}

	close (sd);

}

void *cliente ( void *arg ) {

	int sdc;
	int n;
	char buffer[P_SIZE];
	struct psuma *suma;

	suma = (struct psuma *) buffer;

	sdc = *( (int *) arg);

	printf("Nuevo Cliente:%i \n",sdc);

	n = 1;
	while ( n != 0) {

		// aca debemos hacer la logica del cliente (cliente|worker)

		

	}
	printf("Cliente desconectado \n");
	close (sdc);
	
}

