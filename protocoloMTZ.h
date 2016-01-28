/*
ARCHIVO DE DEFINICION DE PROTOCOLO.
En este archivo se define el protocolo de alto nivel para comunicar nuestra app.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

//CODIGOS DE LOS MENSAJES
//....................................................
//Codigo del tipo 40X - CONFIRMACION ACK
#define ACK 401
//....................................................
//codigos del tipo 10X - SOLICITUDES DE CONEXION.
#define SOLICITUD_CLIENTE 101
#define SOLICITUD_WORKER 102


//DECLARACION DE LA ESTRUCTURA DEL MENSAJE
#define HEADER_LENGHT sizeof(headerMTZ)

//.....................................................
typedef struct header
{
  int codigo;
  uint32_t lenght; //corresponde a la longitud total del mensaje

} headerMTZ;

typedef struct body
{
  char * mensage;
} bodyMTZ;

typedef struct protocolo
{
    headerMTZ header;
    bodyMTZ body;
} protocoloMTZ;

//DEFINICIONES DE LAS FUNCIONES PARA LOS MENSAJES
//.....................................................

int leerBytes(int sd, void *buffer, int len);
int leer_mensaje(int sd, protocoloMTZ *mjs);
uint16_t enviar_mensaje(int sd, int codigo, char * mensajes);
