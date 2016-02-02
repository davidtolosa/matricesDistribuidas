#include "funtionsServer.h"
#include "protocoloMTZ.h"

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
    printf("Error al cargar el cliente\n");
    db_closeDB(handler); // cierro la conexion
    return 0;
    }
  else
    {
    printf("Cliente cargado\n");
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
  printf("Abro la base de datos\n");
  handler = db_openDB(SQLITE_OPEN_READWRITE);
  char query[256];

  sprintf(query, "DELETE FROM cliente WHERE id_cliente=%i;",sdc);
	if(handler !=NULL)
	{
	  if( db_insert_update_delete(handler, query) != SQLITE_OK )
	  {
		printf("Error al eliminar el cliente\n");
		db_closeDB(handler); // cierro la conexion
		return 0;
		}
	  else
		{
		printf("Cliente eliminado\n");
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
    printf("Error al cargar el worker\n");
    db_closeDB(handler); // cierro la conexion
    return 0;
    }
  else
    {
    printf("Worker cargado\n");
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
			printf("Error al eliminar el worker\n");
			db_closeDB(handler); // cierro la conexion
	    return 0;
	    }
		else
			{
			printf("Worker eliminado\n");
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
			printf("No se pudo cargar la operacion\n");
			db_closeDB(handler); // cierro la conexion
			return 0;
		}

		i++;
	}

}

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

      printf("Trabajo Obtenido para el Worker:\n ID SUBOPERACION:%i TIPO:%i VALORES:%s\n",id_suboperacion,tipo_operacion,valores);

      switch (tipo_operacion) {
        case OPERACION_SUMA:
        {
          printf("ENVIAR TRABAJO SUMA\n");
          enviar_mensaje(sdc,ASIGNACION_TRABAJO_SUMA,valores);

		  sqlite3_finalize(stmt);
		  db_closeDB(handle);
          break;
        }
        case OPERACION_RESTA:
        {
          printf("ENVIAR TRABAJO RESTA\n");
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
      printf("No hay trabajo para el worker%i\n",sdc);

      enviar_mensaje(sdc, SIN_TRABAJOS, "Aun no tengo Trabajos");

      return 0;
    }
}

int saveResult(char *resultado, int id_suboperacion)
{
  sqlite3 *handler;
  handler = db_openDB(SQLITE_OPEN_READWRITE);
  char query[256];

  sprintf(query, "UPDATE operaciones SET resultado='%s' WHERE id_suboperacion=%i",resultado,id_suboperacion);

  if( db_insert_update_delete(handler, query) != SQLITE_OK )
  {
    printf("Error al guardar el resultado\n");
    db_closeDB(handler); // cierro la conexion
    return 0;
    }
  else
    {
    printf("Resultado guardado\n");
    db_closeDB(handler); // cierro la conexion
    return 1;
    }
}

int setWorkerOperation(int id_worker,int id_suboperacion)
{
  sqlite3 *handler;
  handler = db_openDB(SQLITE_OPEN_READWRITE);
  char query[256];

  sprintf(query, "UPDATE operaciones SET id_worker=%i WHERE id_suboperacion=%i",id_worker,id_suboperacion);

  if( db_insert_update_delete(handler, query) != SQLITE_OK )
  {
    printf("Error al guardar el worker\n");
    db_closeDB(handler); // cierro la conexion
    return 0;
    }
  else
    {
    printf("Worker guardado\n");
    db_closeDB(handler); // cierro la conexion
    return 1;
    }
}

char* checkEndOperation(int id_cliente){

  sqlite3_stmt *stmt;
  char query[256];
  int retval;

  sqlite3 *handle =db_openDB(SQLITE_OPEN_READONLY);

  sprintf(query,"SELECT id_cliente,fila,resultado FROM operaciones WHERE NOT EXISTS (SELECT * FROM operaciones WHERE id_cliente=%i AND resultado ISNULL) AND id_cliente=%i",id_cliente,id_cliente);

}
