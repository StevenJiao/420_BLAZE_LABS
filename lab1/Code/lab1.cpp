#include <stdio.h>
#include <pthread.h>



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


    int thread_cnt = 4;
    pthread_t threads[thread_cnt];

    for (int i=0; i<thread_cnt; i++) {
        pthread_create(&threads[i], NULL, threadfunc, (void *) i);
    }

    for (int i = 0; i < thread_cnt; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;

}





