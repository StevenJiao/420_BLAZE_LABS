#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

extern "C" {
    #include "lab1_IO.h"
}
#include "timer.h"

int **A; int**B; int** C; int n;
int thread_cnt;

double start, end;

void* threadfunc(void* arg_p){
    int rank = *((int *) arg_p);

    // calculate the row (x) and col (y) for the thread to work on in the matrix
    int x = floor(rank/(int)sqrt(thread_cnt));
    int y = rank % (int)sqrt(thread_cnt);

    int factor = (n/sqrt(thread_cnt));

    for (int i = factor*x; i <= factor*(x+1)-1; i++) {
        for (int j = factor*y; j <= factor*(y+1)-1; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    free(arg_p);
    return nullptr;
}

int largest_factor(int p) {
    // calculate n^2
    int n_squared = n * n;

    // get the maximum number that we count down from; this should be the minimum between p and n
    int min = p <= n ? p : n;
    for (int i = min; i >= 1; i--) {
        // calculate and floor the square root of our current number
        int square_root = (int) sqrt(i);

        // if the number is a factor of n^2, and is a square number, we return it.
        if (n_squared % i == 0 && square_root * square_root == i) return i;
    }

    // if we didn't find one, return a count of 1
    return 1;
}

int main(int argc, char *argv[]) {

    /*
    Spawn a set of pthreads to operate on a set of matrix cells

    -----
    Input:
    int thread_cnt      number of threads to be used

    -----
    Output:

    -----
    Example:
    */

    // Get matrix data
    Lab1_loadinput(&A, &B, &n);

    C = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        C[i] = (int*) malloc(n * sizeof(int));
    }

    // Get the thread count from cmd, initialize threads and return vars
    if (argc < 2)
        return 0;
    thread_cnt = largest_factor(strtol(argv[1], nullptr, 0));

    pthread_t threads[thread_cnt];
    void* return_val;

    // Start timing
    GET_TIME(start);

    // Create each thread, and wait on their return values
    for (int i=0; i<thread_cnt; i++) {
        int *arg = (int *) malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&threads[i], NULL, threadfunc, (void *) arg);
    }

    // Join each thread, get output in return_val
    for (int i = 0; i < thread_cnt; i++) {
        pthread_join(threads[i], &return_val);
    }

    // End timing
    GET_TIME(end)

    // Output result
    Lab1_saveoutput(C, &n, end-start);

    free(A);
    free(B);
    free(C);

    return 0;
}





