#include <stdio.h>
#include <sqlite3.h>

//FUNCIONES PARA EL MANEJO DE LA BD

sqlite3* db_openDB(int flag);

void db_closeDB (sqlite3 *handle);

int db_selectDB(sqlite3 *handle, char *query, sqlite3_stmt  **stmt);

int db_fetchDB(sqlite3_stmt *stmt);

int db_insert_update_delete(sqlite3 *handle, char *query);

void initDB();
