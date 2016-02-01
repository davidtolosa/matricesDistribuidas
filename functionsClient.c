#include "functionsClient.h"
#include "protocoloMTZ.h"

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
  printf("SUMA <FileMTZ1.mtz> <FileMTZ1.mtz> - Suma la A y B\n");
  printf("RESTA <FileMTZ1.mtz> <FileMTZ1.mtz> - Resta la A a la B\n");
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
int showConsole(int sd)
{
	int showConsole=1;
	int exit = 0;
	char teclado[256];

	char* operacion = NULL;
	char *file1  = NULL;
	char *file2  = NULL;
	char* m1 = NULL;
	char* m2 = NULL;
	char* buffer= NULL;

	while( showConsole !=0)
	{


		printf( "$ > ");
		fgets(teclado, sizeof(teclado), stdin);
		teclado[strlen(teclado) - 1] = '\0';
		fflush(stdin);

		operacion = strtok (teclado," ");

		if( strcmp(operacion, "SUMA")==0)
		{
			/*Obtengo las cadenas para poder procesarlas	*/
			file1 = strtok (NULL," ");
			file2 = strtok (NULL," ");
			printf( "FILE 1 : %s\n", file1 );
			printf( "FILE 2 : %s\n", file2 );

			// Intento  abrir los archivos y obtener las matrices
			m1 = obtainMTZ(file1);
			if(m1!=NULL)
			{
				//si el primer archivo esta correcto continuo
				m2 = obtainMTZ(file2);
				if(m2!=NULL)
				{
					//si el segundo archivo esta correcto empiezo a armar el mensaje a enviar
					buffer = createBuffer(m1,m2);
					enviar_mensaje(sd,OPERACION_SUMA,buffer);
					//Salgo del bucle para esperar la respuesta
					showConsole=0;


				}
				else
					{
						printf("No se encuentra el Archivo: %s \n",file2);
					}
			}
			else
				{
					printf("No se encuentra el Archivo: %s \n",file1);
				}

			//--------------------------------------------------


		}
		else
			{
				if( strcmp(operacion, "RESTA")==0)
				{
					/*Obtengo las cadenas para poder procesarlas	*/
					file1 = strtok (NULL," ");
					file2 = strtok (NULL," ");
					printf( "FILE 1 : %s\n", file1 );
					printf( "FILE 2 : %s\n", file2 );
				}
				else if( strcmp(operacion, "SALIR")==0)
				{
					exit=1;
					showConsole=0;

				}
				else if( strcmp(operacion, "HELP")==0)
				{
					system("clear");
					showHelpClient();
				}
				else
					{
						printf("Operacion Invalida\n");
					}

			}


	}

	if( buffer)
		free( buffer);

	if(exit==1)
		return -1;
	else
		return 0;
}

/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/
char* createBuffer(char* M1, char* M2)
{
	char *buffer = NULL;
	int M1_size = strlen(M1)+2;
	int M2_size = strlen(M2)+1;

	buffer = (char *) realloc( buffer , ( sizeof(char)* M1_size ));
	strcpy(buffer,M1);

	buffer[M1_size-2] = ';';


	buffer = (char *) realloc( buffer , ( sizeof(char)* (M1_size+M2_size)));
	strcat(buffer,M2);


	return buffer;


}

/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/
char* obtainMTZ(char* file)
{

	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char *buffer = NULL;

	int counter=1;
	int linesize=0;

	fp = fopen(file, "r");

	if (fp == NULL)
	{
		printf("Archivo inexistente o invalido\n");
		return NULL;
	}
	else{


		while ((read = getline(&line, &len, fp)) !=-1) {

			linesize = strlen(line)+1;
			if(linesize >2)
			{
				buffer = (char*) realloc ( buffer , (sizeof(char)*linesize*counter)); //al buffer asignar la memoria.
				strcat(buffer, line);

				counter++;
			}

		}


		fclose(fp);

		if (line)
			free(line);

		return buffer;
	}
}

int askForWork(int sd){

		enviar_mensaje(sd,SOLICITUD_TRABAJO,"Server Necesito Trabajar");

}
