#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>

#include "timer.h"
#include "Lab3IO.h"

int main(int argc, char* argv[])
{
    int i, j, k, max, max_i, size;
	double** Au;
	double* X;
	double temp;
	int* index;
    double start, end;

    // Ensure we got enough arguements
    if (argc != 2) {
        printf("Error: 2 command line arguments are required. <num_threads>\n"); 
        return 1;
    }

    // Get argument values
    int num_threads = atoi(argv[1]);

	// Load data
	Lab3LoadInput(&Au, &size);
	X = CreateVec(size);

	// Initialize index
	index = malloc(size * sizeof(int));
	for (int i = 0; i < size; ++i)
		index[i] = i;

	// Start timing
    GET_TIME(start);

	// 1x1 array case
	if (size == 1)
		X[0] = Au[0][1] / Au[0][0];
	else {
		
		/*Gaussian elimination*/
		for (int k=0; k<size-1; k++) {
			
			/*Pivoting - Find row with max col k*/
			max = 0;
			max_i = 0;
			for (int i=k; i<size; i++) {
				if (Au[index[i]][k] * Au[index[i]][k] > max) {
					max_i = i;
					max = Au[index[i]][k]*Au[index[i]][k];
				}
			}
			
			/* Pivoting - Swap row k with row having max col k*/
			if (max_i != k) {
				temp = index[max_i];
				index[max_i] = index[k];
				index[k] = temp;
			}

			/*calculating*/
			for (i=k+1; i<size; i++) {
				temp = Au[index[i]][k] / Au[index[k]][k];
				for (j=k; j<size+1; j++) {
					Au[index[i]][j] -= temp * Au[index[k]][j];
				}
			}
		}
		

		/*Jordan elimination*/
		for (k=size-1; k>=1; k--) {
			for (i=0; i<k; i++) {
				Au[index[i]][size] -= Au[index[i]][k] / Au[index[k]][k] * Au[index[k]][size];
				Au[index[i]][k] = 0;
			} 
		}
		/*Solution*/
		for (k=0; k< size; ++k) {
			X[k] = Au[index[k]][size] / Au[index[k]][k];
		}
	}

    // End timing
    GET_TIME(end)
	printf("Main unoptimzed ran in %f seconds with %d thread(s).\n", end-start, num_threads);

    // Save output file
    Lab3SaveOutput(X, size, end-start);

	// Free memory
	DestroyVec(X);
	DestroyMat(Au, size);
	free(index);

    return 0;
}