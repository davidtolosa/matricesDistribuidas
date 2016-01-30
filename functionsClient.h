#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// Este tipo de dato nos permite saber el orden de una matris.
typedef struct orden {
	
	int filas;
	int columnas;
	
}ordenMTZ;

void showHelpClient();


ordenMTZ* obtainMTZ( char *file);

int validateOperation(ordenMTZ *M1 , ordenMTZ *M2);

