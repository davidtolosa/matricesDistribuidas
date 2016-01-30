#include "functionsClient.h"

/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/

void showHelpClient()
	{
  system("clear");
  printf("----------------------------------------------------------------------\n");
  printf("--------------------------Bienvenido a MTZ----------------------------\n");
  printf("----------------------------------------------------------------------\n");
  printf("\n");
  printf("Puede realizar las siguientes operaciones ejecutando los comandos:\n\n");
  printf("SUM <FileMTZ1.mtz> <FileMTZ1.mtz> - Suma la A y B\n");
  printf("RES <FileMTZ1.mtz> <FileMTZ1.mtz> - Resta la A a la B\n");
  printf("HELP Ayuda\n");
  printf("SALIR - Sale del programa\n");
  printf("\n");
  printf("Los resultados seran devueltos en resultado.mtz\n\n");
 
 }

/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/
ordenMTZ* obtainMTZ( char *file)
{
	
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	ordenMTZ *orden;
	
	fp = fopen(file, "r");
	
	if (fp == NULL)
		{	
			printf("Archivo inexistente o invalido\n");
		}
	else{	
			
			
		    while ((read = getline(&line, &len, fp)) != -1) {
				printf("%s", line);
			
			}
	
		}
	
	fclose(fp);
	if (line)
		free(line);
	
	return orden;
	
}

/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/
int validateOperation(ordenMTZ *M1 , ordenMTZ *M2)
{
	
	
}


