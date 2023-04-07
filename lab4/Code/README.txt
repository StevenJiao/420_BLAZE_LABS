NOTE: web-Stanford.txt is not included but is required by the program.

To run this code on a single computer:
1. run the command 'make all && ./datatrim' to make all executables and create the data input files.
2. run the command 'make run num_p=4' to run the program with 4 processes on a single computer.
3. run the command 'make move_to_nodes' to move executables into 

To run this code on a multiple computers:
1. run the command 'make all && ./datatrim' to make all executables and create the data input files.
2. run the command 'make move_to_nodes' to move executables into the home directory of user_07's cluster's slave nodes.
2. run the command 'make run-distrib num_p=4' to run the program with 4 processes on a multiple computers.