#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

#include "lab1_IO.c"
#include "timer.h"

int **A, **B, **C, n;

double start, end;

void* threadfunc (void* arg_p){
    int i = ((int*) arg_p)[0];
    int thread_cnt = ((int*) arg_p)[1];

    // calculate the row (x) and col (y) for the thread to work on in the matrix
    int x = floor(i/(int)sqrt(thread_cnt));
    int y = i % (int)sqrt(thread_cnt);

    // calculate the lower bound and upper bound for i
    

    // calculate lower and upper bound for j
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

    // get the largest number that is both a square and factor of n^2
    int thread_cnt = largest_factor(thread_cnt);

    pthread_t threads[thread_cnt];
    void** return_val;

    // Start timing
    GET_TIME(start);

    // Create each thread, and wait on their return values
    for (int i=0; i<thread_cnt; i++) {
        int input[2] = {i, thread_cnt};

        pthread_create(&threads[i], NULL, threadfunc, (void*) input);
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





