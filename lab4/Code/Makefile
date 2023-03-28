CC = gcc
CXX = g++
C99 = c99
MPI = mpicc
CFLAGS = -Wall
CXXFLAGS = -Wall -std=gnu++0x
MAIN_EXECUTABLE = main
OTHER_EXECUTABLE = serialtester datatrim
GENERATED_DATA_FILES = data_input_link data_input_meta
LAB_NUM = 4

all: main datatrim serialtester

# make run num_p = 4
run:
	mpirun -np ${num_p} ./${MAIN_EXECUTABLE}

main: lab4io.o
	${MPI} ${CFLAGS} main.c Lab4_IO.o -o main -lm

lab4io.o: Lab4_IO.c
	${CC} ${CFLAGS} -c Lab4_IO.c -o Lab4_IO.o

datatrim: datatrim.c
	${CC} datatrim.c -o datatrim

# Generate data_input_link and data_input_meta
generate-data: datatrim
	./datatrim

serialtester:
	${CC} ${CFLAGS} serialtester.c Lab4_IO.c -o serialtester -lm

# will only work when logged into master node
move_to_nodes: main generate-data
	scp -r ${MAIN_EXECUTABLE} ${GENERATED_DATA_FILES} user_07@192.168.1.55:;scp -r ${MAIN_EXECUTABLE} ${GENERATED_DATA_FILES} user_07@192.168.1.88:;scp -r ${MAIN_EXECUTABLE} ${GENERATED_DATA_FILES} user_07@192.168.1.104:;

# Zip files for submission
submission:
	cd ..; zip -r 1535268.zip Code/ Members/ Report/ -x \main \*.o \*.zip \web-Stanford.txt

# Send all files to lab machine for testing
teleport-all: submission
	cd ..; scp -P9007 -r 1535268.zip Code/ Members/ user_07@129.128.215.200:labs/lab$(LAB_NUM)

# Send just main.c to lab machine for testing (faster for lab 4)
teleport-code: all
	scp -P9007 -r main.c user_07@129.128.215.200:labs/lab$(LAB_NUM)/Code

# Remove all intermediate files
clean:
	rm -rf *.o ${GENERATED_DATA_FILES} $(MAIN_EXECUTABLE) $(OTHER_EXECUTABLE)

# Sends updated code to the lab master node and generates data (connection info stored in '420' ssh config)
move: main clean teleport-code
	ssh 420 -f 'cd labs/lab4/Code && make move_to_nodes && exit'

# Runs main through mpi on the master node
run-mpi:
	ssh 420 -f 'cd labs/lab4/Code && make run num_p=4 && exit'