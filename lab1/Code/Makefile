CC=g++
CFLAGS= -O -Wall
LAB1_OBJECTS = lab1.o
# CLIENT_OBJECTS = client.o helpers.o
# SERVER_OBJECTS = server.o helpers.o

lab1: lab1.o
	$(CC) $(CFLAGS) $(LAB1_OBJECTS) -o main

# Create object (.o) files
lab1.o: lab1.cpp
	$(CC) $(CFLAGS) -c lab1.cpp -o lab1.o

utils: matrix serial

# Generate matrix generation executable
matrix:
	gcc matrixgen.c -o matrixgen

# Generate serial tester executable
serial:
	gcc serialtester.c lab1_IO.c -o serialtester

# Remove all intermediate files
clean:
	rm -rf *.o

# Compress for submission
# compress:
# 	zip assignment2.zip Makefile README.md client.h client.cpp server.h server.cpp helpers.h helpers.cpp tands.c