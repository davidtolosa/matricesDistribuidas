#include "functionsClient.h"

void showHelpClient(){
  system("clear");
  printf("----------------------------------------------------------------------\n");
  printf("--------------------------Bienvenido a MTZ----------------------------\n");
  printf("----------------------------------------------------------------------\n");
  printf("\n");
  printf("Puede realizar las siguientes operaciones ejecutando los comandos:\n\n");
  printf("SUM <FileMTZ1.mtz> <FileMTZ1.mtz> - Suma la A y B\n");
  printf("RES <FileMTZ1.mtz> <FileMTZ1.mtz> - Resta la A a la B\n");
  printf("SALIR - Sale del programa\n");
  printf("\n");
  printf("Los resultados seran devueltos en resultado.mtz\n\n");
  }


char** str_split(char* a_str, const char a_delim)
{
	char** result    = 0;
	size_t count     = 0;
	char* tmp        = a_str;
	char* last_comma = 0;
	char delim[2];
	delim[0] = a_delim;
	delim[1] = 0;
	
	/* Count how many elements will be extracted. */
	while (*tmp)
	{
		if (a_delim == *tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}
	
	/* Add space for trailing token. */
	count += last_comma < (a_str + strlen(a_str) - 1);
	
	/* Add space for terminating null string so caller
	knows where the list of returned strings ends. */
	count++;
	
	result = malloc(sizeof(char*) * count);
	
	if (result)
	{
		size_t idx  = 0;
		char* token = strtok(a_str, delim);
		
		while (token)
		{
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;
	}
	
	return result;
}
