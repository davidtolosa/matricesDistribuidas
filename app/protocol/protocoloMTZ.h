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
#define ACK_CLIENTE_REGISTER 401
#define ACK_WORKER_REGISTER 402
#define ACK_OPERACION 403
#define ACK_OPERACION_WORKER 404


//....................................................
//codigos del tipo 10X - SOLICITUDES DE CONEXION.
#define SOLICITUD_CLIENTE 101
#define SOLICITUD_WORKER 102
#define NO_WORKER_DISPONIBLES 103

//....................................................
//Codigo del tipo 20X - OPERACIONES CLIENTE/WORKER
#define OPERACION_SUMA 201
#define OPERACION_RESTA 202
#define RESULTADO_MATRICES 204
//....................................................
//Codigo del tipo 30X - WORKER
#define SOLICITUD_TRABAJO 301
#define ASIGNACION_TRABAJO_SUMA 302
#define ASIGNACION_TRABAJO_RESTA 304
#define RESULTADO_TRABAJO 305
#define SIN_TRABAJOS 306

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
