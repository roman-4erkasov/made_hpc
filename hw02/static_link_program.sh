gcc -c linalg.c
gcc -c score.c
ar rc static_lib.a linalg.o score.o
gcc main.c static_lib.a -o static.exe
