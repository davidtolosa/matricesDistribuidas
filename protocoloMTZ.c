/*
ARCHIVO DE DEFINICION DE PROTOCOLO.
En este archivo se define el protocolo de alto nivel para comunicar nuestra app.
*/
#include "protocoloMTZ.h"

int leerBytes(int sd, void *buffer, int len)
{
    int byte = 1;
    int leido = 0;

    while ( ( leido < len) && ( byte != 0 ) ) {
        byte = recv(sd, buffer+leido, (len-leido) , 0);
        if (byte < 0) {
             perror("Error en recv");
             exit(EXIT_FAILURE);
        }else {
          if (byte == 0)
            return byte;
        }

        leido += byte;
    }
    return (leido);
}

int leer_mensaje(int sd, protocoloMTZ *mjs )
{
    int n;

    /*char * buffer=NULL;*/
	headerMTZ head;
	bodyMTZ body;

    printf("Mensaje para leer \n");
	n = leerBytes (sd, &head , HEADER_LENGHT );
	printf("Leido header: %i \n",n);

	if(n !=0 )
	{
		mjs->header.codigo = head.codigo;
		mjs->header.lenght = head.lenght;
		
		printf("--------------------------------\n");
		printf("Mensaje a Recibido HEADER: \n");
		printf("Codigo : %i \n",mjs->header.codigo );
		printf("Longitud : %i \n",mjs->header.lenght);
		printf("--------------------------------\n");


		if (mjs->header.lenght > 0){
/*			buffer = (char *) malloc (sizeof(char)*(mjs->header.lenght +1));*/
/*			if (buffer == NULL){*/
/*				perror ( "No se puede asignar memoria" );*/
/*				exit(EXIT_FAILURE);*/
/*			}*/
/*			memset(buffer,0, mjs->header.lenght+1);*/
			printf("Hay mas por leer\n");
			if (n != 0) {
				
					n = leerBytes (sd, &body, mjs->header.lenght - HEADER_LENGHT);
					printf("Leido body : %i \n",n );
					if (n != 0) {
						mjs->body.mensage = body.mensage;
						printf("Body : %s \n",mjs->body.mensage );
						printf("--------------------------------\n");
				
						fflush(stdout);
						return n;
						}
					else {
						fflush(stdout);
						return n;
					}
			}
		}
	}
	else
		{
			fflush(stdout);
			return (n);
		}

}

uint16_t enviar_mensaje(int sd, int codigo, char * mensajes)
{
	int n;
	char *buffer;
	int size=0;
	//creo el mensaje a enviar
	protocoloMTZ mensaje;

	mensaje.header.codigo=codigo;
	mensaje.body.mensage = mensajes;
	mensaje.header.lenght = sizeof(mensaje);
	
  uint32_t  lon= sizeof(mensaje); //longitud total del mensaje

  mensaje.header.lenght = htons( sizeof(mensaje));
  
  
  printf("--------------------------------\n");
  printf("Mensaje a Enviar: \n");
  printf("Codigo : %i \n",mensaje.header.codigo);
  printf("Longitud : %i \n",mensaje.header.lenght);
  printf("Body : %s \n",mensaje.body.mensage);
  printf("lenght : %i \n",lon);
  printf("--------------------------------\n");
  
  
  
  //reservo el tamamo del buffer a enviar
  buffer = (char *) malloc ((sizeof(char) * (lon+1) ));

  if (buffer== NULL){
        perror ( "No se puede asignar memoria: " );
        exit(EXIT_FAILURE);
    }

  memset(buffer,0,(sizeof(char) * (lon+1)));
  memcpy ( buffer , &mensaje.header , HEADER_LENGHT);	 // Guarda al inicio del buffer el código y longitud del mensaje
  memcpy ( buffer + HEADER_LENGHT, &mensaje.body , lon ); // Por último guarda el mensaje

  printf ("\nDatos a enviar:\n CODIGO: %d \n LONGITUD: %u \n DATOS: %s\n\n", mensaje.header.codigo, mensaje.header.lenght, mensaje.body.mensage);
  fflush(stdout);

  n = send (sd, buffer, lon  , 0);	// envia los datos!

  free(buffer);

  return (n);
}
