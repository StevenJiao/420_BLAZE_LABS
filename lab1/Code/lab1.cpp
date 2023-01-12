#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

#include "lab1_IO.c"
#include "timer.h"

int **A, **B, **C, n;

double start, end;

void* threadfunc (void* arg_p){}

int main(int argc , char *argv[]) {

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

    // Get the thread count from cmd, initialize threads and return vars
    int thread_cnt = strtol(argv[1], nullptr, 0);
    pthread_t threads[thread_cnt];
    void** return_val;

    // Start timing
    GET_TIME(start);

    // Create each thread, and wait on their return values
    for (int i=0; i<thread_cnt; i++) {
        // calculate the row(x) and col(y) for the thread to work on in the matrix
        int x = floor(i/(int)sqrt(thread_cnt));
        int y = i % (int)sqrt(thread_cnt);

        pthread_create(&threads[i], NULL, threadfunc, (void *) i);
    }

    // Join each thread, get output in return_val
    for (int i = 0; i < thread_cnt; i++) {
        pthread_join(threads[i], return_val);
    }

    // End timing
    GET_TIME(end)

    // Output result
    Lab1_saveoutput(C, &n, end-start);

    return 0;
}





