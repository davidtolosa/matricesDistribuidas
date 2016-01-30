# matricesDistribuidas

Compilar SERVER

	gcc -g -o ./build/server tcp_server_threads.c protocoloMTZ.c DBManager.c functionsServer.c -l pthread -l sqlite3
	
	CORRERLO
		./build/server localhost


Compilar CLIENTE

	gcc -o ./build/cliente tcp_client.c protocoloMTZ.c  functionsClient.c
	
	CORRERLO
		./build/cliente localhost cliente
		./build/cliente localhost worker



