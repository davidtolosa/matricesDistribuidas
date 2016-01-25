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
   

    char * buffer=NULL;

    n = leerBytes (sd, &mjs->header , HEADER_LENGHT );
	
	if(n !=0 )
	{	
		//mjs->header.codigo = msj->header.codigo;
		mjs->header.lenght = ntohs( mjs->header.lenght);


		if (mjs->header.lenght > 0){
			buffer = (char *) malloc (sizeof(char)*(mjs->header.lenght +1));
			if (buffer == NULL){
				perror ( "No se puede asignar memoria" );
				exit(EXIT_FAILURE);
			}
			memset(buffer,0, mjs->header.lenght+1);

			if (n != 0) {
				n = leerBytes (sd, buffer, mjs->header.lenght);
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
    char *buffer;
	
	//creo el mensaje a enviar
	protocoloMTZ mensaje;
	
	mensaje.header.codigo=codigo;
	mensaje.body.mensage = mensajes;
	mensaje.header.lenght = sizeof(mensaje);
	
	
    uint32_t  lon= sizeof(mensaje); //longitud total del mensaje

    mensaje.header.lenght = htons( sizeof(mensaje));
    //reservo el tamamo del buffer a enviar
    buffer = (char *) malloc ((sizeof(char) * (lon+1) ));
    if (buffer== NULL){
        perror ( "No se puede asignar memoria: " );
        exit(EXIT_FAILURE);
    }
    memset(buffer,0,(sizeof(char) * (lon+1)));
    memcpy ( buffer , &mensaje.header , HEADER_LENGHT);	 // Guarda al inicio del buffer el código y longitud del mensaje
    memcpy ( buffer + HEADER_LENGHT, &mensaje.body , lon ); // Por último guarda el mensaje

    n = send (sd, buffer, lon  , 0);	// envia los datos!

   free(buffer);

    return (n);
}
