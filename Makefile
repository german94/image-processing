CC=g++
CFLAGS=-c -Wall -Wextra -Werror -ansi -pedantic

all: main.o myMatrix.o fileHandler.o
	$(CC) main.o myMatrix.o fileHandler.o -o testMatrix
	
noWarning: main.cpp myMatrix.cpp fileHandler.cpp
	$(CC) main.cpp myMatrix.cpp fileHandler.cpp -o testMatrix

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
	
myMatrix.o: myMatrix.cpp
	$(CC) $(CFLAGS) myMatrix.cpp
	
fileHandler.o: fileHandler.cpp
	$(CC) $(CFLAGS) fileHandler.cpp
	
clean:
	rm -f main.o myMatrix.o fileHandler.o testMatrix

run:
	./testMatrix
