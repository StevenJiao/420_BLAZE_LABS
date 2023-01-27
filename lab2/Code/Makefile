CC = gcc
CXX = g++
C99 = c99
CFLAGS = -Wall -c
CXXFLAGS = -Wall -std=gnu++0x
MAIN_EXECUTABLE = main1 main2 main3 main4
OTHER_EXECUTABLE = attacker client
LAB_NUM = 2
main: main1 main3 main4 #main2

# Link and create executable
main1: main1.o
	$(CXX) main1.o -o main1 -lpthread

main1.o: main1.cpp
	$(CXX) $(CXXFLAGS) -c main1.cpp -o main1.o

main2: main2.o
	$(CXX) main2.o -o main2 -lpthread

main2.o: main2.cpp
	$(CXX) $(CXXFLAGS) -c main2.cpp -o main2.o

main3: main3.o
	$(CXX) main3.o -o main3 -lpthread

main3.o: main3.cpp
	$(CXX) $(CXXFLAGS) -c main3.cpp -o main3.o

main4: main4.o
	$(CXX) main4.o -o main4 -lpthread

main4.o: main4.cpp
	$(CXX) $(CXXFLAGS) -c main4.cpp -o main4.o

attacker:
	${C99} attacker.c -o attacker -lpthread

client:
	${C99} client.c -o client -lpthread

# Zip files for submission
submission:
	cd ..; zip -r 1535268.zip Code/ Members/ -x \*main \*.o \*.zip

# Send files to lab machine for testing
teleport: submission
	cd ..; scp -P9007 -r 1535268.zip Code/ Members/ user_07@129.128.215.200:labs/lab$(LAB_NUM)

# Remove all intermediate files
clean:
	rm -rf *.o $(MAIN_EXECUTABLE) $(OTHER_EXECUTABLE)