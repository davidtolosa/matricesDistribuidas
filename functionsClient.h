#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>


void showHelpClient();

int showConsole(int sd);
	
char* createBuffer(char* M1, char* M2);

char* obtainMTZ(char* file);
