CC=g++
CFLAGS=-fopenmp
LOPT=

SOURCES = main.cpp matrix.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = app.exe

all: main

main: main.o matrix.o
	$(CC) $(CFLAGS) main.o matrix.o $(LOPT) -o $(EXECUTABLE)

matrix.o: matrix.cpp
	$(CC) $(CFLAGS) -c matrix.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm -rf *.o *.a *.exe

