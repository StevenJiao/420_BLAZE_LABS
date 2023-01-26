CC = gcc
CXX = g++
CFLAGS = -Wall -c
CXXFLAGS = -Wall -std=gnu++0x
LAB1_OBJECTS = 
EXECUTABLE = main


# Zip files for submission
submission:
	cd ..; zip -r 1535268.zip Code/ Members/ -x \*main \*.o \*.zip

# Send files to lab machine for testing
teleport: submission
	cd ..; scp -P9007 -r 1535268.zip user_07@129.128.215.200:labs/

# Remove all intermediate files
clean:
	rm -rf *.o $(EXECUTABLE)