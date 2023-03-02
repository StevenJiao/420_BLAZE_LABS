#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<omp.h>

#include "timer.h"
#include "Lab3IO.h"

int main(int argc, char* argv[])
{
    int i, j, k, size, max, max_i;
	double** Au;
	double* X;
	double swap_temp, temp;
	int* index;
    double start, end;
	int row_max, row_max_i;

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
			
			// Reset row max
			row_max = -1;

			#pragma omp parallel default(none) shared(row_max, row_max_i, size, k, Au, index, swap_temp) private(max, max_i) num_threads(num_threads)
			{
				max = 0;
				max_i = 0;
				#pragma omp for
				for (int i=k; i<size; i++) {
					if (Au[index[i]][k] * Au[index[i]][k] > max) {
						max_i = i;
						max = Au[index[i]][k]*Au[index[i]][k];
					}
				}

				#pragma omp critical
				{
					if (max > row_max) {
						row_max = max;
						row_max_i = max_i;
					}
				}

				/* Pivoting - Swap row k with row having max col k */
				#pragma omp single
				{
					if (row_max_i != k) {
						// printf("row max = %d\n", row_max);
						swap_temp = index[row_max_i];
						index[row_max_i] = index[k];
						index[k] = swap_temp;
					}
				}  // Implicit barrier here

				/* Calculating */
				#pragma omp for private(i, j, temp)
				for (i=k+1; i<size; i++) {
					temp = Au[index[i]][k] / Au[index[k]][k];
					for (j=k; j<size+1; j++) {
						Au[index[i]][j] -= temp * Au[index[k]][j];
					}
				}

			}
		}
		
		/*Jordan elimination*/
		#pragma omp parallel for num_threads(num_threads) 
		for (k=size-1; k>=1; k--) {
			for (i=0; i<k; i++) {
				Au[index[i]][size] -= Au[index[i]][k] / Au[index[k]][k] * Au[index[k]][size];
				Au[index[i]][k] = 0;
			} 
		}
		
		/*Solution*/
		#pragma omp parallel for num_threads(num_threads) 
		for (k=0; k< size; ++k) {
			X[k] = Au[index[k]][size] / Au[index[k]][k];
		}
	}

    // End timing
    GET_TIME(end)
	printf("Main optimized 1 ran in %f seconds with %d thread(s).\n", end-start, num_threads);

    // Save output file
    Lab3SaveOutput(X, size, end-start);

	// Free memory
	DestroyVec(X);
	DestroyMat(Au, size);
	free(index);

    return 0;
}