/**
 * Main1: Best version
*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<omp.h>

#include "timer.h"
#include "Lab3IO.h"

int main(int argc, char* argv[])
{

    int i, j, k, size, local_max_i, local_max, max_i, max;
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

    /*Load the datasize and verify it*/
	Lab3LoadInput(&Au, &size);

    /*Calculate the solution by serial code*/
	X = CreateVec(size);
	index = malloc(size * sizeof(int));
	for (i = 0; i < size; ++i)
		index[i] = i;

    // Start timing
    GET_TIME(start);

		// 1x1 array case
	if (size == 1)
		X[0] = Au[0][1] / Au[0][0];
	else {
		#pragma omp parallel num_threads(num_threads) default(none) shared(index, Au, size, X, max_i, max) private(j, k, temp, i, local_max_i, local_max)
		{
			/*Gaussian elimination*/
			for (k = 0; k < size - 1; ++k){
				/*Pivoting*/
                max = 0;
                max_i = 0;
                #pragma omp for
                for (i= k; i < size; ++i)
                    if (max < Au[index[i]][k] * Au[index[i]][k]){
                        local_max = Au[index[i]][k] * Au[index[i]][k];
                        local_max_i = i;
                    }

                #pragma omp critical
                {
                    if (local_max > max) {
                        max = local_max;
                        max_i = local_max_i;
                    }
                }
                
                #pragma omp barrier

                #pragma omp single
                {
                    if (max_i != k)/*swap*/{
                        i = index[max_i];
                        index[max_i] = index[k];
                        index[k] = i;
                    }
                }
                
				/*calculating*/
				#pragma omp for
				for (i = k + 1; i < size; ++i){
					temp = Au[index[i]][k] / Au[index[k]][k];
					for (j = k; j < size + 1; ++j)
						Au[index[i]][j] -= Au[index[k]][j] * temp;
				}       
	
			}

			/*Jordan elimination*/
			for (k = size - 1; k > 0; --k){
				#pragma omp for
				for (i = k - 1; i >= 0; --i ){
					temp = Au[index[i]][k] / Au[index[k]][k];
					Au[index[i]][k] -= temp * Au[index[k]][k];
					Au[index[i]][size] -= temp * Au[index[k]][size];
				} 
			}

			/*solution*/
			#pragma omp for
			for (k=0; k< size; ++k)
				X[k] = Au[index[k]][size] / Au[index[k]][k];
		}
	}

    // End timing
    GET_TIME(end)
	printf("Main optimized 1 ran in %f seconds with %d thread(s).\n", end-start, num_threads);

    // Save output file
    Lab3SaveOutput(X, size, end-start);

    return 0;
}