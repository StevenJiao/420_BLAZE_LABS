CC = gcc
CXX = g++
CFLAGS = -Wall -c
CXXFLAGS = -Wall -std=gnu++0x
LAB1_OBJECTS = lab1.o lab1_IO.o
LAB_NUM = 1
EXECUTABLE = main

# Link and create executable
lab1: ${LAB1_OBJECTS}
	$(CXX) $(LAB1_OBJECTS) -o $(EXECUTABLE) -lpthread

# Create object (.o) files
lab1.o: lab1.cpp
	$(CXX) $(CXXFLAGS) -c lab1.cpp -o lab1.o

lab1_IO.o: lab1_IO.c
	${CC} ${CFLAGS} -c lab1_IO.c -o lab1_IO.o

# Generate matrixgen and serialtester executables
utils: matrix serial

matrix:
	${CC} matrixgen.c -o matrixgen

serial:
	${CC} serialtester.c lab1_IO.c -o serialtester

# Zip files for submission
submission:
	cd ..; zip -r 1535268.zip Code/ Members/ -x \*main \*matrixgen \*serialtester \*data_input \*data_output \*.o \*.zip

# Send files to lab machine for testing
teleport: submission
	cd ..; scp -P9007 -r 1535268.zip Code/ Members/ user_07@129.128.215.200:labs/lab$(LAB_NUM)

# Remove all intermediate files
clean:
	rm -rf *.o $(EXECUTABLE)