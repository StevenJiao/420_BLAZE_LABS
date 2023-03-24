CC = gcc
CXX = g++
C99 = c99
MPI = mpicc
CFLAGS = -Wall
CXXFLAGS = -Wall -std=gnu++0x
MAIN_EXECUTABLE = main
OTHER_EXECUTABLE = serialtester datatrim
GENERATED_DATA_FILES = 
LAB_NUM = 4

all: main datatrim serialtester

# make run num_p = 4
run:
	mpirun -np ${num_p} ./${MAIN_EXECUTABLE}

main:
	${MPI} ${CFLAGS} main.c Lab4_IO.c -o main -lm

datatrim: datatrim.o
	${CC} ${CFLAGS} datatrim.o -o datatrim

datatrim.o: datatrim.c
	${CC} ${CFLAGS} -c datatrim.c

serialtester:
	${CC} ${CFLAGS} serialtester.c Lab4_IO.c -o serialtester -lm

# will only work when logged into master node
move_to_nodes:
	scp -r ${MAIN_EXECUTABLE} user_07@192.168.1.55:;scp -r ${MAIN_EXECUTABLE} user_07@192.168.1.88:;scp -r ${MAIN_EXECUTABLE} user_07@192.168.1.104:;

# Zip files for submission
submission:
	cd ..; zip -r 1535268.zip Code/ Members/ Report/ -x \main \*.o \*.zip \web-Stanford.txt

# Send files to lab machine for testing
teleport: submission
	cd ..; scp -P9007 -r 1535268.zip Code/ Members/ user_07@129.128.215.200:labs/lab$(LAB_NUM)

# Remove all intermediate files
clean:
	rm -rf *.o ${GENERATED_DATA_FILES} $(MAIN_EXECUTABLE) $(OTHER_EXECUTABLE)

# Temporary
move: main clean teleport
	ssh 420 -f 'cd labs/lab4/Code && make move_to_nodes && exit'

r:
	ssh 420 -f 'cd labs/lab4/Code && make run num_p=4 && exit'