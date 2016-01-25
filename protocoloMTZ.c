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

        leido += ret;
    }
    return (leido);
}

int leer_mensaje(int sd, protocoloMTZ ** mjs )
{
    int n;
    headerMTZ *head;
    bodyMTZ *body;
    protocoloMTZ *mensaje;


    char * buffer=NULL;

    n = leerBytes (sd, &head , HEADER_LENGHT );
    mjs.header.codigo = head.codigo;
	mjs.header.lenght =ntohs( head.lenght);

/*
    if (encabezado.longitud > 0){
        buffer = (char *) malloc (sizeof(char)*(encabezado.longitud +1));
        if (buffer == NULL){
            perror ( "No se puede asignar memoria" );
            exit(EXIT_FAILURE);
        }
        memset(buffer,0, encabezado.longitud+1);

        if (n != 0) {
            n = leerBytes (PE_sd, buffer, encabezado.longitud);
        }
    }
*/
  fflush(stdout);


    return (1);
}


uint16_t enviar_mensaje(int sd, protocoloMTZ *mensaje);
{
    int n;
    char *buffer;
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
