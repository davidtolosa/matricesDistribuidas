# matricesDistribuidas

Ejemplos de como compilar los archivos. Posteriormente lo vamos acomodar para compilar nuestra APP

1.- gcc hola.c
compila el programa en C hola.c, gener un archivo ejecutable a.out.
2.-  gcc -o hola hola.c
compila el programa en C hola.c, gener un archivo ejecutable hola.
3.-  g++ -o hola hola.cpp
compila el programa en C++ hola.c, gener un archivo ejecutable hola.
4.-  gcc -c hola.c
no genera el ejecutable, sino el código objeto, en el archivo hola.o. Si no s indica un nombre para el archivo objeto, usa el nombre del archivo en C y le cambia la extensión por .o.
5.-  gcc -c -o objeto.o hola.c
genera el código objeto indicando el nombre de archivo.
6.-  g++ -c hola.cpp
igual para un programa en C++.
7.-  g++ -o ~/bin/hola hola.cpp
genera el ejecutable hola en el subdirectorio bin del directorio propio del usuario.
8.-  g++ -L/lib -L/usr/lib hola.cpp
indica dos directorios donde han de buscarse bibliotecas. La opción -L debe repetirse para cada directorio de búsqueda de bibliotecas.
9.-  g++ -I/usr/include hola.cpp
indica un directorio para buscar archivos de encabezado (de extensión .h).
