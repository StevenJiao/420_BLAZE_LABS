#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


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

    printf("It works!\n");

    // Get the thread count from cmd, initialize threads and return vars
    int thread_cnt = strtol(argv[1], nullptr, 0);
    pthread_t threads[thread_cnt];
    void** return_val;

    // Create each thread, and wait on their return values
    for (int i=0; i<thread_cnt; i++) {
        pthread_create(&threads[i], NULL, threadfunc, (void *) i);
        pthread_join(threads[i], return_val);

    }

    return 0;
}





