#define LAB4_EXTEND

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#include "Lab4_IO.h"

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

#define THRESHOLD 0.0001

struct node *nodehead;
int nodecount;
double *r, *last_r, *my_r, *contribution;
int i, j;
double damp_const;
int iterationcount = 0;
double start, end;
int processes, my_rank;
FILE *ip;

int nodes_per_process;
int my_nodecount, my_nodestart, my_nodeend;
double error;

int read_input() {

    // Read the input file
    if ((ip = fopen("data_input_meta","r")) == NULL) {
        printf("Error opening the data_input_meta file.\n");
        return 1;
    }

    // Get node count
    fscanf(ip, "%d\n", &nodecount);
    fclose(ip);

    // Initialize the graph
    if (node_init(&nodehead, 0, nodecount)) return 1;

}

// Setup for a single worker
int init(int argc, char* argv[]) {

    // Init MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Get graph info from input file
    if (read_input()) return 1;

    // Initialize info for this process
    my_nodecount = nodes_per_process;
    my_nodestart = my_rank * nodes_per_process;
    my_nodeend = my_rank * nodes_per_process + (my_rank == 0);

    // Initialize r vectors
    r = malloc(nodecount * sizeof(double));
    my_r = malloc(nodecount * sizeof(double));
    last_r = malloc(nodecount * sizeof(double));

    // Set starting values for the shared and local rank vectors, r_i(0) = 1/N
    for ( i = 0; i < nodecount; ++i)
        last_r[i] = 0.0;
        r[i] = 1.0 / nodecount;
        // Update only the local nodes that this process handles
        if (i < my_nodecount) {
            my_r[i] = r[i];
        }

    return 0;
}

// One iteration of the page rank calculation
int iteration() {

    // TODO: Calculate updates to my_r here

    // TODO: Update other processes
    MPI_Allgatherv();

}

int main(int argc, char* argv[]) {

    // Setup each process and then start timing
    init(&argc, &argv);
    GET_TIME(start);

    do {
        iteration();
        // TODO: Calc error - rel_error(r, last_r, nodecount)
        // TODO: Update last_r - vec_cp(r, last_r, nodecount);

    } while (error >= EPSILON);

    GET_TIME(end);

    // Save output if master process
    if (my_rank == 0) {
        printf("Program converged at %d th iteration - Elapsed time %f - Process %d out of %d\n", iterationcount, end-start, myrank + 1, processes);
        Lab4_saveoutput(r, nodecount, end-start);
    }

    // Cleanup    
    MPI_Finalize();
    node_destroy(nodehead, nodecount);
    free(contribution);

    return 0;
}