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
  printf("\n\nLos resultados seran devueltos en resultado.mtz\n\n");
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
					//END SUMA
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
							enviar_mensaje(sd,OPERACION_RESTA,buffer);
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
							//END RESTA
							//--------------------------------------------------
				}
				else if( strcmp(operacion, "SALIR")==0)
				{
					exit=1;
					showConsole=0;

					//--------------------------------------------------
					//END RESTA
					//--------------------------------------------------
				}
				else if( strcmp(operacion, "HELP")==0)
				{
					system("clear");
					showHelpClient();

					//--------------------------------------------------
					//END HELP
					//--------------------------------------------------
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

			while ((read = getline(&line, &len, fp)) !=-1)
				{
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

/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/

int askForWork(int sd)
{
	enviar_mensaje(sd,SOLICITUD_TRABAJO,"Estoy listo para trabajar");
}

/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/

char** split_delim (char* string,  char*  delim)
{
	char** result =0;

	char* s_temp=0;

	char* fila_aux=0;
	char* fila_aux_temp=0;
	int count_fila=0;

	s_temp = strdup(string);
	//Cuento cuantos delimitadores tengo
/*	fila_aux = strtok(s_temp, delim);*/
/*	while(fila_aux != NULL)*/
/*	{*/
/*		if ( strlen(fila_aux) > 1)*/
/*			count_fila++;*/

/*		fila_aux = strtok(0,delim);*/
/*	}*/
	while (*s_temp)
	{
		if(*s_temp==*delim)
			count_fila++;

		s_temp++;
	}

	//Reservo la memoria para los vectores
	result = malloc (sizeof(char) * count_fila + strlen(string)*3);

	//ahora estraigo cada cadena y la guardo en el vector resultado
	count_fila=0;

	fila_aux_temp = strtok(string, delim);

	while(fila_aux_temp != NULL)
	{
		if ( strlen(fila_aux_temp) > 0)
		{
				*(result + count_fila ) = strdup(fila_aux_temp);
				count_fila++;
			}

		fila_aux_temp = strtok(0, delim);
	}

	return result;
}
/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/

char* solverOperation (char *values, int op)
{
	char* vec1 = strtok(values,";");
	char* vec2 = strtok(NULL,";");

	printf("VEC 1 : %s \n", vec1);
	printf("VEC 2 : %s \n", vec2);

	char** elements_m1 =NULL;
	char** elements_m2 =NULL;

	char * result = NULL;
	char element_result[256];

	int count = 0;
	//obtengo las filas de las matrices
	elements_m1 = split_delim(vec1 , ",");
	elements_m2 = split_delim(vec2 , ",");
	float value = 0;
	int size = 1;

	int num_elem = 0;
	while (*(elements_m1 + num_elem) != NULL)
	{
		num_elem++;
	}

	while (*(elements_m1 + count) != NULL)
	{

		//convierto
		if(op == ASIGNACION_TRABAJO_SUMA)
			value  = atof(*(elements_m1 + count)) + atof(*(elements_m2 + count));

		if(op == ASIGNACION_TRABAJO_RESTA)
			value  = atof(*(elements_m1 + count)) - atof(*(elements_m2 + count));

		//creo la cadena con el elemnto resultado
		if(count == (num_elem-1))
			sprintf(element_result, "%.2f" , value);
		else
			sprintf(element_result, "%.2f," , value);
		//voy calculando el nuevo tama�o del vector resultado
		size += sizeof(char) * (strlen(element_result)+1);
		//solicito mas memoria para el resultado
		result = (char * ) realloc (result, size );
		//concateno los valores
		printf("FILA %i  :A %s  +- B %s = %f \n",  count ,*(elements_m1 + count),*(elements_m2 + count),value);

		strcat(result,element_result);

		count++;
	}
		printf ("resultado: %s \n",result);
		return result;
}

/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/
int saveResult(char* result)
{
	FILE * fp;
	
	
	fp = fopen("./resultado.mtz", "w+");
	
	if (fp == NULL)
	{
		printf("Archivo inexistente o invalido\n");
		return -1;
	}
	else{
			fputs(result,fp);
			return 1;
	}
	
	
}

/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/

void *progresBar ( void *arg )
{
	
	while(1)
	{
		
		//system("clear");
		printf("\033[A\033[K");		
		printf ("Processing\n");
		fflush(stdout);
		sleep(1);
		
		//system("clear");
		printf("\033[A\033[K");
		printf ("Processing.\n");
		fflush(stdout);
		sleep(1);
		printf("\033[A\033[K");
		//system("clear");
		printf ("Processing..\n");
		fflush(stdout);
		sleep(1);
		printf("\033[A\033[K");
		//system("clear");
		printf ("Processing...\n");
		fflush(stdout);
		sleep(1);
		//system("clear");
		printf("\033[A\033[K");
		printf ("Processing..\n");
		fflush(stdout);
		sleep(1);
		//system("clear");
		
		
	}
	
}
