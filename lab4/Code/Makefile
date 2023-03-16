CC = gcc
CXX = g++
C99 = c99
CFLAGS = -Wall
CXXFLAGS = -Wall -std=gnu++0x
MAIN_EXECUTABLE = 
OTHER_EXECUTABLE = 
GENERATED_DATA_FILES = 
LAB_NUM = 4

compile:

datatrim:
	${CC} datatrim.c -o datatrim

serialtester: serialtester.c
	${CC} serialtester.c Lab4IO.c -o serialtester -lm

move_to_nodes:
	scp -r 

# Zip files for submission
submission:
	cd ..; zip -r 1535268.zip Code/ Members/ Report/ -x \*main \*.o \*.zip

# Send files to lab machine for testing
teleport: submission
	cd ..; scp -P9007 -r 1535268.zip Code/ Members/ user_07@129.128.215.200:labs/lab$(LAB_NUM)

# Remove all intermediate files
clean:
	rm -rf *.o ${GENERATED_DATA_FILES} $(MAIN_EXECUTABLE) $(OTHER_EXECUTABLE)