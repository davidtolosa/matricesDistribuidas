# matricesDistribuidas

Compilar SERVER

	gcc -o ../build/server tcp_server_threads.c ../protocol/protocoloMTZ.c functionsServer.c ../db/DBManager.c -l pthread -l sqlite3

	
	CORRERLO
		./build/server localhost


Compilar CLIENTE

	gcc -o ../build/cliente tcp_client.c functionsClient.c ../protocol/protocoloMTZ.c -l pthread

	
	CORRERLO
		./build/cliente localhost cliente
		./build/cliente localhost worker


SUMA ./file/matrizA.mtz ./file/matrizB.mtz

