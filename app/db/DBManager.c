/*
 FUNCIONES PARA LA UTILIZACION DE LA DB (SQLITE3).
*/
#include "DBManager.h"

/*
FUNCION
Descipcion : Se encarga de realizar la conexion a la BD
Nombre :  db_openDB
Recibe: flag = SQLITE_OPEN_READONLY | SQLITE_OPEN_READWRITE
Retorna: Puntero a la conexion que se creo.
*/
sqlite3* db_openDB(int flag){

	sqlite3 *handle;

	int retval = sqlite3_open_v2("../db/dbMTZ.sqlite3",&handle,flag,0);

	sqlite3_exec(handle, "PRAGMA foreign_keys = ON;", 0, 0, 0);

	if(retval!=SQLITE_OK)
	{
		#ifdef DEBUG
		printf("Error conexion '%s'.\n", sqlite3_errmsg(handle));
		#endif
		db_closeDB(handle);
	}
	else
	  {
		#ifdef DEBUG
		printf("Open database.\n");
		#endif
		return handle;
	  }
}

/*
FUNCION
Descipcion : Cierra una conexion previamente instanciada.
Nombre :  db_closeDB
Recibe:  puntero al manejador sqlite *handle
Retorna: void
*/

void db_closeDB (sqlite3 *handle)
{
  sqlite3_close(handle);
}

/*
FUNCION
Descipcion : Realiza una consulta del tipo SELECT contra la DB.
Nombre :  db_selectDB
Recibe:  puntero al manejador sqlite *handle, cadena de consulta y un puntero
        al valor de retorno del tipo sqlite3_stmt *.
Retorna: int
*/

int db_selectDB(sqlite3 *handle, char *query, sqlite3_stmt  **stmt)
{
	srand((unsigned)time(0));
  int retval,i;

  retval = sqlite3_prepare_v2(handle, query, -1, stmt, 0);

  while (retval == SQLITE_BUSY) {
		#ifdef DEBUG
		printf("--> BUSSY sqlite3_prepare_v2\n ");
		#endif

		retval = rand()%999999;
  	for(i=0; i<retval; i++);
  		retval = sqlite3_prepare_v2(handle, query, -1, stmt, 0);
  }

  return retval;
}

int db_fetchDB( sqlite3_stmt *stmt){

  int retval,i;

  retval = sqlite3_step(stmt);
  while (retval == SQLITE_BUSY) {
		#ifdef DEBUG
    printf("--> BUSSY sqlite3_step\n ");
		#endif
    retval = rand()%999999;
    for(i=0; i<retval; i++);
    retval = sqlite3_step(stmt);
  }

  return retval;
}

/*
FUNCION
Descipcion : Realiza una consulta del tipo INSERT|UPDATE|DELETE contra la DB.
Nombre :  db_selectDB
Recibe:  puntero al manejador sqlite *handle y cadena de consulta
Retorna: int
*/
int db_insert_update_delete(sqlite3 *handle, char *query){

  srand((unsigned)time(0));
  int retval,i;
  char *error = 0;

  retval = sqlite3_exec(handle, query, 0, 0, &error);

  while (retval == SQLITE_BUSY) {
    retval = rand()%1000;
    for(i=0; i<retval; i++);

    retval = sqlite3_exec(handle, query, 0, 0, 0);
    if (retval!= SQLITE_OK)
    {
				#ifdef DEBUG
        printf("Error Operacion '%s'.\n",error);
				#endif
		}

  }

  return retval;
}

/*
FUNCION
Descipcion : Inicializo la BD, elimino todos los datos de las tablas.
Nombre : initBD()
Recibe:
Retorna:
*/

void initDB(){

  sqlite3 *handle;
  int retval;
  char query[256];

  handle = db_openDB(SQLITE_OPEN_READWRITE);

  	sqlite3_exec(handle, "BEGIN", 0, 0, 0);

    sprintf(query, "DELETE FROM cliente;");
		retval = db_insert_update_delete(handle, query);

		sprintf(query, "DELETE FROM worker;");
		retval = db_insert_update_delete(handle, query);

		sprintf(query, "DELETE FROM operaciones;");
		retval = db_insert_update_delete(handle, query);

		sqlite3_exec(handle, "END", 0, 0, 0);

  db_closeDB(handle);
}
