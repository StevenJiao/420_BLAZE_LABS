CC = gcc-12
CXX = g++
C99 = c99
CFLAGS = -Wall
CXXFLAGS = -Wall -std=gnu++0x
MAIN_EXECUTABLE = main
OTHER_EXECUTABLE = serialtester datatrim
LAB_NUM = 4

all: main lab4io.o datatrim serialtester

main: main.o
	${CXX} main.o -o main

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

lab4io.o: Lab4_IO.c
	${CC} ${CFLAGS} -c Lab4_IO.c -o Lab4_IO.o

datatrim: datatrim.c
	${CC} ${CFLAGS} datatrim.c -o datatrim

serialtester: serialtester.c
	${CC} serialtester.c Lab4_IO.c -o serialtester -lm

# Zip files for submission
submission:
	cd ..; zip -r 1535268.zip Code/ Members/ Report/ -x \*main \*.o \*.zip

# Send files to lab machine for testing
teleport: submission
	cd ..; scp -P9007 -r 1535268.zip Code/ Members/ user_07@129.128.215.200:labs/lab$(LAB_NUM)

# Remove all intermediate files
clean:
	rm -rf *.o ${GENERATED_DATA_FILES} $(MAIN_EXECUTABLE) $(OTHER_EXECUTABLE)