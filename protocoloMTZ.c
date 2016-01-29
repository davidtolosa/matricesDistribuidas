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
  headerMTZ head;
  bodyMTZ body;
	char * buffer=NULL;

  n = leerBytes (sd, &head , HEADER_LENGHT );
	printf("Leido header: %i \n",n);

  if(n !=0 )
	{
		mjs->header.codigo = head.codigo;
		mjs->header.lenght = head.lenght;

		printf("--------------------------------\n");
		printf("Mensaje a Recibido HEADER: Size(%i)\n",HEADER_LENGHT);
		printf("Codigo : %i \n",mjs->header.codigo );
		printf("Longitud : %i \n",mjs->header.lenght);

    if (mjs->header.lenght > 0){
/*			buffer = (char *) malloc (sizeof(char)*(mjs->header.lenght +1));*/
/*			if (buffer == NULL){*/
/*				perror ( "No se puede asignar memoria" );*/
/*				exit(EXIT_FAILURE);*/
/*			}*/
/*			memset(buffer,0, mjs->header.lenght+1);*/
					buffer = (char *) malloc(mjs->header.lenght - HEADER_LENGHT);
					memset(buffer,0, mjs->header.lenght - HEADER_LENGHT);

					n = leerBytes (sd, buffer, mjs->header.lenght - HEADER_LENGHT);

          if (n != 0) {
						printf("Body %s: \n", buffer);
						printf("--------------------------------\n\n");
						mjs->body.mensage = (char *) malloc (sizeof(char)* strlen(buffer));
						strcpy(mjs->body.mensage, buffer);
						fflush(stdout);
						return n;
					}
					else {
						fflush(stdout);
						return n;
					}
			}
	}
	else{
			fflush(stdout);
			return (n);
		}
}

uint16_t enviar_mensaje(int sd, int codigo, char * mensajes)
{
	int n;
	char *buffer = NULL;
	char *datos = NULL;
	int size=0;
	protocoloMTZ mensaje; //creo el mensaje a enviar

  mensaje.header.codigo=codigo;
  
  //preparo los datos para poder ser enviados y armo el paquete.
  datos = malloc( sizeof(char) * strlen(mensajes));
  memset(datos,0, strlen(mensajes));
  sprintf(datos,"%s",mensajes);
  //---------------------------------------------
  mensaje.body.mensage = datos;

  uint32_t  lon =  HEADER_LENGHT + (sizeof(char)*((strlen(mensaje.body.mensage) + 1))); //longitud total del mensaje
  mensaje.header.lenght = lon;
 // mensaje.header.lenght = htons( sizeof(mensaje));
  printf("--------------------------------\n");
  printf("Mensaje a Enviar: \n");
  printf("Codigo : %i \n",mensaje.header.codigo);
  printf("Longitud : %i - %i - %u\n",mensaje.header.lenght, lon,htons( lon ));
  printf("Body : %s \n",mensaje.body.mensage);
  printf("--------------------------------\n");

  buffer = (char *) malloc (lon);

  if (buffer== NULL){
        perror ( "No se puede asignar memoria: " );
        exit(EXIT_FAILURE);
    }

  memset(buffer,0,lon);
  memcpy ( buffer , &mensaje.header , HEADER_LENGHT);	 // Guarda al inicio del buffer el código y longitud del mensaje
  memcpy ( buffer + HEADER_LENGHT, mensaje.body.mensage , lon ); // Por último guarda el mensaje
  fflush(stdout);

  n = send (sd, buffer, lon  , 0);	// envia los datos!
  printf("Datos enviados : %i\n", n);
  
  free(buffer);

  return (n);
}
