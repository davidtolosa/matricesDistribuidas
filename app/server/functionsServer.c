#include "funtionsServer.h"
#include "../protocol/protocoloMTZ.h"

/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/

int newClient(int sdc){
  sqlite3 *handler;
  handler = db_openDB(SQLITE_OPEN_READWRITE);
  char query[256];

  sprintf(query, "INSERT INTO cliente (id_cliente) VALUES (%i);",sdc);

  if( db_insert_update_delete(handler, query) != SQLITE_OK )
  {
    #ifdef DEBUG
    printf("Error al cargar el cliente\n");
    #endif
    db_closeDB(handler); // cierro la conexion
    return 0;
    }
  else
    {
    #ifdef DEBUG
    printf("Cliente cargado\n");
    #endif
    db_closeDB(handler); // cierro la conexion
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


int deleteClient(int sdc){

  sqlite3 *handler =NULL;
  #ifdef DEBUG
  printf("Abro la base de datos\n");
  #endif
  handler = db_openDB(SQLITE_OPEN_READWRITE);
  char query[256];

  sprintf(query, "DELETE FROM cliente WHERE id_cliente=%i;",sdc);
	if(handler !=NULL)
	{
	  if( db_insert_update_delete(handler, query) != SQLITE_OK )
	  {
    #ifdef DEBUG
		printf("Error al eliminar el cliente\n");
    #endif
    db_closeDB(handler); // cierro la conexion
		return 0;
		}
	  else
		{
    #ifdef DEBUG
		printf("Cliente eliminado\n");
    #endif
    db_closeDB(handler); // cierro la conexion
		return 1;
		}
	}
	else
	{
		return 0;
	}
}

/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/


int newWorker(int sdc){
  sqlite3 *handler;
  handler = db_openDB(SQLITE_OPEN_READWRITE);
  char query[256];

  sprintf(query, "INSERT INTO worker (id_worker) VALUES (%i);",sdc);

  if( db_insert_update_delete(handler, query) != SQLITE_OK )
  {
    #ifdef DEBUG
    printf("Error al cargar el worker\n");
    #endif
    db_closeDB(handler); // cierro la conexion
    return 0;
    }
  else
    {
    #ifdef DEBUG
    printf("Worker cargado\n");
    #endif
    db_closeDB(handler); // cierro la conexion
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

int deleteWorker(int sdc){

	sqlite3 *handler;
	handler = db_openDB(SQLITE_OPEN_READWRITE);
	char query[256];

	sprintf(query, "DELETE FROM worker WHERE id_worker=%i;",sdc);
		if( db_insert_update_delete(handler, query) != SQLITE_OK )
		{
      #ifdef DEBUG
			printf("Error al eliminar el worker\n");
      #endif
      db_closeDB(handler); // cierro la conexion
	    return 0;
	    }
		else
			{
      #ifdef DEBUG
			printf("Worker eliminado\n");
      #endif
      db_closeDB(handler); // cierro la conexion
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

char** split_delim (char* string,  char* delim)
{
	char** result =0;

	char* s_temp=0;

	char* fila_aux=0;
	char* fila_aux_temp=0;
	int count_fila=0;

	s_temp = strdup(string);
	//Cuento cuantos delimitadores tengo
	fila_aux = strtok(s_temp, delim);
	while(fila_aux != NULL)
	{
		if ( strlen(fila_aux) > 2)
			count_fila++;

		fila_aux = strtok(0,delim);
	}

	//Reservo la memoria para los vectores

	result = malloc (sizeof(char) * count_fila + strlen(string));

	//ahora estraigo cada cadena y la guardo en el vector resultado

	count_fila=0;

	fila_aux_temp = strtok(string, delim);

	while(fila_aux_temp != NULL)
	{
		if ( strlen(fila_aux_temp) > 2)
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

int createOperation(char *buffer, int id_cli, int id_ope)
{
  int i=0;
//variables para la BD
	sqlite3 *handler;
	handler = db_openDB(SQLITE_OPEN_READWRITE);
	char query[256];

//obtengo las matrices
	char* mat1 = strtok(buffer,";");
	char* mat2 = strtok(NULL,";");
	char** filas_m1 =NULL;
	char** filas_m2 =NULL;
//obtengo las filas de las matrices
	filas_m1 = split_delim(mat1 , "\n");
	filas_m2 = split_delim(mat2 , "\n");

  while(*(filas_m1 + i) != NULL)
	{
		sprintf(query, "INSERT INTO operaciones (tipo_operacion, valores,fila,id_cliente) VALUES (%i,'%s;%s',%i,%i);",id_ope,*(filas_m1 + i),*(filas_m2 + i),i,id_cli);

		if( db_insert_update_delete(handler, query) != SQLITE_OK )
		{
      #ifdef DEBUG
			printf("No se pudo cargar la operacion\n");
      #endif
      db_closeDB(handler); // cierro la conexion
			return 0;
		}

		i++;
	}
}
/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/

int getSendWork(int sdc,int *id_suboper)
{
  sqlite3_stmt *stmt;
  char query[256];
  int id_suboperacion;
  int tipo_operacion;
  char *valores;
  int retval;

  sqlite3 *handle =db_openDB(SQLITE_OPEN_READONLY);

  sprintf(query,"SELECT id_suboperacion,tipo_operacion,valores FROM operaciones WHERE id_worker ISNULL AND resultado ISNULL ORDER BY RANDOM() LIMIT 1;");
  retval = db_selectDB(handle, query, &stmt);
  retval = db_fetchDB(stmt);

  if (retval == SQLITE_ROW) {

      id_suboperacion = sqlite3_column_int(stmt, 0);
      tipo_operacion = sqlite3_column_int(stmt,1);
      valores = (char *) malloc(strlen((const char*) sqlite3_column_text(stmt,2))+1);
      strcpy(valores, (const char*) sqlite3_column_text(stmt,2));

      #ifdef DEBUG
      printf("Trabajo Obtenido para el Worker:\n ID SUBOPERACION:%i TIPO:%i VALORES:%s\n",id_suboperacion,tipo_operacion,valores);
      #endif

      switch (tipo_operacion) {
        case OPERACION_SUMA:
        {
          #ifdef DEBUG
          printf("ENVIAR TRABAJO SUMA\n");
          #endif
          enviar_mensaje(sdc,ASIGNACION_TRABAJO_SUMA,valores);

		      sqlite3_finalize(stmt);
		      db_closeDB(handle);
          break;
        }
        case OPERACION_RESTA:
        {
          #ifdef DEBUG
          printf("ENVIAR TRABAJO RESTA\n");
          #endif
          enviar_mensaje(sdc,ASIGNACION_TRABAJO_RESTA,valores);

		      sqlite3_finalize(stmt);
		      db_closeDB(handle);

          break;
        }
        default:
        {
			    sqlite3_finalize(stmt);
			    db_closeDB(handle);

			    break;
        }
      }

      //ACTUALIZO QUE WORKER ESTA RELIZANDO LA OPERACION
      setWorkerOperation(sdc,id_suboperacion);

      *id_suboper = id_suboperacion;

      return 1;
    } else {

	  sqlite3_finalize(stmt);
      db_closeDB(handle);

	    //si no hay trabajos respondo con un mensaje informando que no hay.
      #ifdef DEBUG
      printf("No hay trabajo para el worker%i\n",sdc);
      #endif
      enviar_mensaje(sdc, SIN_TRABAJOS, "Aun no tengo Trabajos");

      return 0;
    }
}
/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/

int saveResult(char *resultado, int id_suboperacion)
{
  sqlite3 *handler;
  handler = db_openDB(SQLITE_OPEN_READWRITE);
  char query[256];

  sprintf(query, "UPDATE operaciones SET resultado='%s' WHERE id_suboperacion=%i",resultado,id_suboperacion);

  if( db_insert_update_delete(handler, query) != SQLITE_OK )
  {
    #ifdef DEBUG
    printf("Error al guardar el resultado\n");
    #endif
    db_closeDB(handler); // cierro la conexion
    return 0;
    }
  else
    {
    #ifdef DEBUG
    printf("Resultado guardado\n");
    #endif
    db_closeDB(handler); // cierro la conexion
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

int setWorkerOperation(int id_worker,int id_suboperacion)
{
  sqlite3 *handler;
  handler = db_openDB(SQLITE_OPEN_READWRITE);
  char query[256];

  sprintf(query, "UPDATE operaciones SET id_worker=%i WHERE id_suboperacion=%i",id_worker,id_suboperacion);

  if( db_insert_update_delete(handler, query) != SQLITE_OK )
  {
    #ifdef DEBUG
    printf("Error al guardar el worker\n");
    #endif
    db_closeDB(handler); // cierro la conexion
    return 0;
    }
  else
    {
    #ifdef DEBUG
    printf("Worker guardado\n");
    #endif
    db_closeDB(handler); // cierro la conexion
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

char* checkEndOperation(int id_cliente){

  sqlite3_stmt *stmt;
  char query[256];
  int retval;

  char* resultado = 0;
  int size = 0;

  sqlite3 *handle =db_openDB(SQLITE_OPEN_READONLY);

  sprintf(query,"SELECT id_cliente,fila,resultado FROM operaciones WHERE NOT EXISTS (SELECT * FROM operaciones WHERE id_cliente=%i AND resultado ISNULL) AND id_cliente=%i ORDER BY fila ASC",id_cliente,id_cliente);

  if( (retval = db_selectDB(handle, query, &stmt)) != SQLITE_ERROR)
	{
		while((retval = db_fetchDB(stmt)) == SQLITE_ROW )
		{
			size+= sizeof(char)* (strlen ( sqlite3_column_text(stmt,2)+2))*2;
			resultado = realloc (resultado , size);

			strcat(resultado , sqlite3_column_text(stmt,2));
			strcat(resultado , "\n");
    }

		if( retval != SQLITE_DONE ) {
      #ifdef DEBUG
			printf("ERROR : %i \n",retval);
      #endif
      sqlite3_finalize(stmt);
			db_closeDB(handle);
      #ifdef DEBUG
			printf("Ocurrio un error con la BD. \n");
      #endif
    }
    return resultado;
  }
  else
	  {
		  sqlite3_finalize(stmt);
		  db_closeDB(handle);
      #ifdef DEBUG
		  printf("Ocurrio un error con la BD. \n");
      #endif
      return resultado;
	  }
}
/*
FUNCION
Descipcion :
Nombre :
Recibe:
Retorna:
*/


int disconnetAllClient()
{
	sqlite3_stmt *stmt;
	char query[256];
	int retval;
	
	char* resultado = 0;
	int size = 0;
	
	sqlite3 *handle =db_openDB(SQLITE_OPEN_READONLY);
	
	sprintf(query,"SELECT id_cliente FROM cliente;");
	
	if( (retval = db_selectDB(handle, query, &stmt)) != SQLITE_ERROR)
	{
		while((retval = db_fetchDB(stmt)) == SQLITE_ROW )
		{
			//obtengo todos los descriptores de la DB y empiezo a desconectarlos
			enviar_mensaje(sqlite3_column_int(stmt,0), NO_WORKER_DISPONIBLES, "Hola Cliente. No hay workers disponibles, intente mas tarde.\n");
			
		}
		
		if( retval != SQLITE_DONE ) {
			#ifdef DEBUG
			printf("ERROR : %i \n",retval);
			#endif
			sqlite3_finalize(stmt);
			db_closeDB(handle);
			#ifdef DEBUG
			printf("Ocurrio un error con la BD. \n");
			#endif
		}
		return 1;
	}
	else
		{
			sqlite3_finalize(stmt);
			db_closeDB(handle);
			#ifdef DEBUG
			printf("Ocurrio un error con la BD. \n");
			#endif
			return 0;
		}
	
	
}
