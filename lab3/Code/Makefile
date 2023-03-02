CC = gcc-12
CXX = g++
C99 = c99
CFLAGS = -Wall
CXXFLAGS = -Wall -std=gnu++0x
MAIN_EXECUTABLE = main_unopt main_opt1 main_opt2 main_opt3 main_opt4
OTHER_EXECUTABLE = serialtester datagen
GENERATED_DATA_FILES = data_input data_output 
LAB_NUM = 3

compile: main_opt1 main_opt2 main_opt3 main_opt4 datagen serialtester

# e.g. 'make run threads=5'
# Runs with 1 thread first then num_threads so we can compare
run: compile
	./datagen -s ${arr_size}
	./main_opt1 ${threads}
	./main_opt2 ${threads}
	./main_opt3 ${threads}
	./main_opt4 ${threads}
	./serialtester

main_unopt: main_unopt.o lab3io.o
	$(CC) main_unopt.o Lab3IO.o -o main_unopt -fopenmp

main_unopt.o: main_unopt.c
	$(CC) $(CFLAGS) -c main_unopt.c -o main_unopt.o -fopenmp

main_opt1: main_opt1.o lab3io.o
	$(CC) main_opt1.o Lab3IO.o -o main_opt1 -fopenmp

main_opt1.o: main_opt1.c
	$(CC) $(CFLAGS) -c main_opt1.c -o main_opt1.o -fopenmp

main_opt2: main_opt2.o lab3io.o
	$(CC) main_opt2.o Lab3IO.o -o main_opt2 -fopenmp

main_opt2.o: main_opt2.c
	$(CC) $(CFLAGS) -c main_opt2.c -o main_opt2.o -fopenmp

main_opt3: main_opt3.o lab3io.o
	$(CC) main_opt3.o Lab3IO.o -o main_opt3 -fopenmp

main_opt3.o: main_opt3.c
	$(CC) $(CFLAGS) -c main_opt3.c -o main_opt3.o -fopenmp

main_opt4: main_opt4.o lab3io.o
	$(CC) main_opt4.o Lab3IO.o -o main_opt4 -fopenmp

main_opt4.o: main_opt4.c
	$(CC) $(CFLAGS) -c main_opt4.c -o main_opt4.o -fopenmp

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