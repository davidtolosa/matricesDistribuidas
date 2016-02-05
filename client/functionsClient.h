#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>



void showHelpClient();
void *showConsole(void *arg);
char* createBuffer(char* M1, char* M2);
char* obtainMTZ(char* file);
int askForWork(int sd);
char** split_delim (char* string,  char* delim);
char* solverOperation (char *values, int op);
int saveResult(char* result);
void *progresBar (void *arg); //THREAD
void showWorkerInfo();
