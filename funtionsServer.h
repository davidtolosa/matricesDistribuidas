#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "DBManager.h"

//DEFINICIONES DE LAS FUNCIONES EL SERVER
//.....................................................

int newClient(int sdc);
int deleteClient(int sdc);
int newWorker(int sdc);
int deleteWorker(int sdc);