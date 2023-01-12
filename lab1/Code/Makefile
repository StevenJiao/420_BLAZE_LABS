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

# Generate executables and PDFs of man pages
# all: both man

# # Generate only executables
# both: client server

# # Generate only the client executable
# client: client.o helpers.o
# 	$(CC) $(CFLAGS) $(CLIENT_OBJECTS) -o client

# # Generate only the server executable
# server: server.o helpers.o
# 	$(CC) $(CFLAGS) $(SERVER_OBJECTS) -o server

# # Create object (.o) files
# client.o: client.cpp client.h
# 	$(CC) $(CFLAGS) -c client.cpp -o client.o

# server.o: server.cpp server.h tands.c
# 	$(CC) $(CFLAGS) -c server.cpp -o server.o

# helpers.o: helpers.cpp helpers.h
# 	$(CC) $(CFLAGS) -c helpers.cpp -o helpers.o

# # Generate PDFs of the man pages for the server and client executables
# man:
# 	groff -Tpdf -man server.1 > server.pdf
# 	groff -Tpdf -man client.1 > client.pdf

# Remove all intermediate files
clean:
	rm -rf *.o

# Compress for submission
# compress:
# 	zip assignment2.zip Makefile README.md client.h client.cpp server.h server.cpp helpers.h helpers.cpp tands.c