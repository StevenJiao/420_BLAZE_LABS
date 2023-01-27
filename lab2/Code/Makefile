CC = gcc
CXX = g++
CFLAGS = -Wall -c
CXXFLAGS = -Wall -std=gnu++0x
MAIN_EXECUTABLE = main1 main2 main3 main4
OTHER_EXECUTABLE = attacker client
main: main1 main3 main4 #main2

# Link and create executable
main1: main1.o
	$(CXX) main1.o -o main1

main1.o: main1.cpp
	$(CXX) $(CXXFLAGS) -c main1.cpp -o main1.o

main2: main2.o
	$(CXX) main2.o -o main2

main2.o: main2.cpp
	$(CXX) $(CXXFLAGS) -c main2.cpp -o main2.o

main3: main3.o
	$(CXX) main3.o -o main3

main3.o: main3.cpp
	$(CXX) $(CXXFLAGS) -c main3.cpp -o main3.o

main4: main4.o
	$(CXX) main4.o -o main4

main4.o: main4.cpp
	$(CXX) $(CXXFLAGS) -c main4.cpp -o main4.o

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
	rm -rf *.o $(MAIN_EXECUTABLE) $(OTHER_EXECUTABLE)