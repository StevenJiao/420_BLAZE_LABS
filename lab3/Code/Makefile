CC = gcc
CXX = g++
C99 = c99
CFLAGS = -Wall -c
CXXFLAGS = -Wall -std=gnu++0x
MAIN_EXECUTABLE = main
OTHER_EXECUTABLE = serialtester
LAB_NUM = 2

# Link and create executable
main: main.o lab3io.o
	$(CXX) main.o Lab3IO.o -o main -lpthread

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

lab3io.o: Lab3IO.c
	${CC} ${CFLAGS} -c Lab3IO.c -o Lab3IO.o

serialtester: serialtester.c
	${CC} serialtester.c Lab3IO.c -o serialtester -lm

# Zip files for submission
submission:
	cd ..; zip -r 1535268.zip Code/ Members/ Report/ -x \*main \*.o \*.zip

# Send files to lab machine for testing
teleport: submission
	cd ..; scp -P9007 -r 1535268.zip Code/ Members/ user_07@129.128.215.200:labs/lab$(LAB_NUM)

# Remove all intermediate files
clean:
	rm -rf *.o $(MAIN_EXECUTABLE) $(OTHER_EXECUTABLE)