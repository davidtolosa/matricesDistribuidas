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
