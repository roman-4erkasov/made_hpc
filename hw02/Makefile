
# $@ - is a macro that refers to target
# $< - is a macro that refers to the first dependency
# $^ - is a macro that refers to all dependecies
# % - is a macro to make a pattern that we want to 
#     watch in both the target and the dependency 


CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=$(shell find ./ ! -name "main.c" -name "*.c")
OBJECTS=$(SOURCES:.c=.o)	
EXECUTABLE=app.exe
LIB=lib.a

all: $(EXECUTABLE)
$(EXECUTABLE): $(LIB)
	$(CC) main.c $(LIB) -o $(EXECUTABLE)
$(LIB): $(OBJECTS)
	ar rc $@ $^
$(OBJECTS): $(SOURCES)
	gcc -c $^
clean:
	rm -f *.o *.a *.exe


