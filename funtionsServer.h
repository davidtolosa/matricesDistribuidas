#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "DBManager.h"

//DEFINICIONES DE LAS FUNCIONES EL SERVER
//.....................................................

int newClient(int sdc);
int deleteClient(int sdc);
int newWorker(int sdc);
int deleteWorker(int sdc);
int getSendWork(int sdc, int *id_suboperacion);
char** split_delim (char* string,  char* delim);
int createOperation(char *buffer, int id_cli, int id_ope);
int saveResult(char *resultado, int id_suboperacion);
int setWorkerOperation(int id_worker,int id_suboperacion);
char* checkEndOperation(int id_cliente);
