CC = gcc-12
CXX = g++
C99 = c99
CFLAGS = -Wall
CXXFLAGS = -Wall -std=gnu++0x
MAIN_EXECUTABLE = main
OTHER_EXECUTABLE = serialtester datagen
GENERATED_DATA_FILES = data_input data_output
LAB_NUM = 3

all: main datagen serialtester

# e.g. 'make run threads=5'
# Runs with 1 thread first then num_threads so we can compare
run:
	./datagen
	./main 1
	./main ${threads}
	./serialtester

main: main.o lab3io.o
	$(CC) main.o Lab3IO.o -o main -fopenmp

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o -fopenmp

lab3io.o: Lab3IO.c
	${CC} ${CFLAGS} -c Lab3IO.c -o Lab3IO.o

datagen: datagen.c
	${CC} ${CFLAGS} datagen.c Lab3IO.c -o datagen

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
	rm -rf *.o ${GENERATED_DATA_FILES} $(MAIN_EXECUTABLE) $(OTHER_EXECUTABLE)