#include "funtionsServer.h"

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

int createOperation(char *buffer, int id_cliente, int operacion)
{

	sqlite3 *handler;
	handler = db_openDB(SQLITE_OPEN_READWRITE);
	char query[256];
	int fila=1;

	//obtengo las dos matrices
	char* mat1 = strtok(buffer,";");
	char* mat2 = strtok(NULL,";");

	printf("M1 : %s \n", mat1);
	printf("M2 : %s \n", mat2);

	char *M1_line = strtok(mat1,"\n");
	char *M2_line = strtok(mat2,"\n");

	while  ((M1_line != NULL))
	{
    printf("FILA:%i:%s\n",fila,M1_line);
    printf("FILA:%i:%s\n",fila,M1_line);

		sprintf(query, "INSERT INTO operaciones (id_cliente,valores, tipo_operacion,fila) VALUES (%i, '%s;%s',%i,%i);",id_cliente,M1_line,M2_line,operacion,fila);

		if( db_insert_update_delete(handler, query) != SQLITE_OK )
		{
			printf("Error al cargar las operacionesn");
			db_closeDB(handler); // cierro la conexion
			return 0;
		}


		M1_line = strtok(NULL,"\n");
		M2_line = strtok(NULL,"\n");
		fila++;
	}

	return 1;
}

int getSendWork(int sdc)
{
  sqlite3_stmt *stmt;
  char query[256];
  int id_suboperacion;
  int tipo_operacion;
  char *valores;

  sqlite3 *handle =db_conectar(SQLITE_OPEN_READONLY);

  sprintf(query,"SELECT id_suboperacion,tipo_operacion,valores FROM operaciones WHERE id_worker ISNULL AND resultado ISNULL ORDER BY RANDOM() LIMIT 1;");
  int retval = sqlite3_prepare_v2(handle, query, -1, &stmt, 0);

  if(retval) {
    printf("Error %i al obtener un trabajo para el Worker.\n",retval);
  }

  retval = sqlite3_step(stmt);

  if (retval == SQLITE_ROW) {

      id_suboperacion = sqlite3_column_int(stmt, 0);
      tipo_operacion = sqlite3_column_int(stmt,1);
      valores = (char *) malloc(strlen((const char*) sqlite3_column_text(stmt,2))+1);
      strcpy(valores, (const char*) sqlite3_column_text(stmt,2));

      printf("Trabajo Obtenido para el Worker:\n ID SUBOPERACION:%i TIPO:%i VALORES:%s\n",id_suboperacion,tipo_operacion,valores);

      return 1;
    } else {

      sqlite3_finalize(stmt);
      db_desconectar(handle);

      return 0;
    }

}
