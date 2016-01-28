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
		
		printf("Leido %i de %i\n Bytes %i\n", leido,len,byte);
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
		printf("--------------------------------\n");


		if (mjs->header.lenght > 0){
/*			buffer = (char *) malloc (sizeof(char)*(mjs->header.lenght +1));*/
/*			if (buffer == NULL){*/
/*				perror ( "No se puede asignar memoria" );*/
/*				exit(EXIT_FAILURE);*/
/*			}*/
/*			memset(buffer,0, mjs->header.lenght+1);*/
					printf("Hay mas por leer - Caracteres : %i\n", (mjs->header.lenght - HEADER_LENGHT) );
					buffer = (char *) malloc(((mjs->header.lenght - HEADER_LENGHT)+1));
					memset(buffer,0, mjs->header.lenght - HEADER_LENGHT);
				
					n = leerBytes (sd, buffer, mjs->header.lenght - HEADER_LENGHT);
					
				
					
					if (n != 0) {
						printf("Body %s:  STRLEN(%i) \n", buffer, strlen(buffer));
						fflush(stdout);
						return n;
						}
					else {
						fflush(stdout);
						return n;
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
	char *buffer = NULL;
	int size=0;
	//creo el mensaje a enviar
	protocoloMTZ mensaje;

	mensaje.header.codigo=codigo;
	mensaje.body.mensage = mensajes;
	
	
  uint32_t  lon =  HEADER_LENGHT + (strlen(mensaje.body.mensage) + 1); //longitud total del mensaje
  mensaje.header.lenght = lon;
 // mensaje.header.lenght = htons( sizeof(mensaje));
  
  
  printf("--------------------------------\n");
  printf("Mensaje a Enviar: \n");
  printf("Codigo : %i \n",mensaje.header.codigo);
  printf("Longitud : %i - %i - %u\n",mensaje.header.lenght, lon,htons( lon ));
  printf("Body : %s \n",mensaje.body.mensage);
  printf("body lenght : %i - STRLEN(%i)\n",sizeof(mensaje.body),strlen(mensaje.body.mensage));
  printf("--------------------------------\n");
  printf("Buffer : %i \n",lon);
  
  
  
  
  //reservo el tamamo del buffer a enviar
  buffer = (char *) malloc (lon);

  if (buffer== NULL){
        perror ( "No se puede asignar memoria: " );
        exit(EXIT_FAILURE);
    }

  memset(buffer,0,lon);
  memcpy ( buffer , &mensaje.header , HEADER_LENGHT);	 // Guarda al inicio del buffer el código y longitud del mensaje
  memcpy ( buffer + HEADER_LENGHT, &mensaje.body , lon ); // Por último guarda el mensaje
  fflush(stdout);

  n = send (sd, buffer, lon  , 0);	// envia los datos!

  free(buffer);

  return (n);
}
