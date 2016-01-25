/*
ARCHIVO DE DEFINICION DE PROTOCOLO.
En este archivo se define el protocolo de alto nivel para comunicar nuestra app.
*/
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

char leer_mensaje(int sd, char ** PS_mensaje)
{
    int n;
    struct mensaje_struct encabezado;
    char * buffer=NULL;

    n = leerBytes (sd, &encabezado, LEN_ENCABEZADO );
    encabezado.codigo = encabezado.codigo;
    encabezado.longitud = ntohs( encabezado.longitud );

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

    if (debug) printf ("Se recibio:\n CODIGO: %c \n LONGITUD: %d \n DATOS: %s\n\n", encabezado.codigo, encabezado.longitud , buffer );
    fflush(stdout);
    *PS_mensaje=buffer;

    return (encabezado.codigo);
}
