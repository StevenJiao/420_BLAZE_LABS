CC = gcc
CXX = g++
CFLAGS = -Wall -c
CXXFLAGS = -Wall -std=gnu++0x
EXECUTABLE = main1 main2 main3 main4
main: main1 # main2 main3 main4

# Link and create executable
main1: main1.o
	$(CXX) main1.o -o main1

main1.o: main1.cpp
	$(CXX) $(CXXFLAGS) -c main1.cpp -o main1.o

main3: main3.o
	$(CXX) main3.o -o main3

main3.o: main3.cpp
	$(CXX) $(CXXFLAGS) -c main3.cpp -o main3.o

attacker:
	${CC} attacker.c -o attacker

client:
	${CC} client.c -o client

# Zip files for submission
submission:
	cd ..; zip -r 1535268.zip Code/ Members/ -x \*main \*.o \*.zip

# Send files to lab machine for testing
teleport: submission
	cd ..; scp -P9007 -r 1535268.zip user_07@129.128.215.200:labs/

# Remove all intermediate files
clean:
	rm -rf *.o $(EXECUTABLE)