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
sqlite3 * db_openDB(int flag){

  sqlite3 *handle;

  int retval = sqlite3_open_v2("DB-Server.db3",&handle,flag,0);


  if(retval)
  {
    printf("Error conexion '%s'.\n", sqlite3_errmsg(handle));
    db_closeDB(handle);
  }

  return handle;
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
	printf("--> BUSSY sqlite3_prepare_v2\n ");
    retval = rand()%999999;
    for(i=0; i<retval; i++);
    retval = sqlite3_prepare_v2(handle, query, -1, stmt, 0);
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
        printf("Error Operacion '%s'.\n",error);
    }

  }

  return retval;
}
