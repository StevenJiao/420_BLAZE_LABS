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
int nodecount, total_elements;
double *r, *last_r, *contribution;
double *my_r, *my_contribution;
int i, j;
double damp_const;
int iterationcount = 0;
double start, end;
int processes, my_rank;
int continue_flag = 1;
FILE *ip;

int nodes_per_process;
int my_nodestart, my_nodeend;

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

    return 0;
}

// Setup for a single worker
int init(int argc, char* argv[]) {

    // Init MPI
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Get graph and nodecount info from input file
    if (read_input()) return 1;

    // set our nodecount for each process (as well as ceiling of the nodes per process so we don't miss one)
    nodes_per_process = ((int) (nodecount / processes)) + (nodecount % processes != 0);
    // set the start and end i for this process
    my_nodestart = nodes_per_process * my_rank;
    my_nodeend = nodes_per_process * (my_rank + 1);
    // set total number of elements since we may not perfectly divide the nodes equally for all processes, there will be extras
    total_elements = nodes_per_process * processes;

    printf("total: %i, processes: %i, nodes per: %i, start: %i, end: %i\n", nodecount, processes, nodes_per_process, my_nodestart, my_nodeend);

    // Initialize r vectors
    r = malloc(total_elements * sizeof(double));
    last_r = malloc(total_elements * sizeof(double));
    // stores local r values for each process
    my_r = malloc(nodes_per_process * sizeof(double));
    // Set starting values for the shared and local rank vectors, r_i(0) = 1/N
    for ( i = 0; i < nodecount; ++i) {
        r[i] = 1.0 / nodecount;
    }

    // initialize contribution sum for j in D_i
    contribution = malloc(total_elements * sizeof(double));
    // stores local contribution values for each process
    my_contribution = malloc(nodes_per_process * sizeof(double));
    for ( i = 0; i < nodecount; ++i)
        contribution[i] = r[i] / nodehead[i].num_out_links * DAMPING_FACTOR;
    damp_const = (1.0 - DAMPING_FACTOR) / nodecount;

    return 0;
}

// One iteration of the page rank calculation
void iteration() {
    // calculate process's chunk of r_i
    for (i = my_nodestart; i < my_nodeend && i < nodecount; ++i) {
        my_r[i - my_nodestart] = damp_const;
        for (j = 0; j < nodehead[i].num_in_links; ++j) {
            my_r[i - my_nodestart] += contribution[nodehead[i].inlinks[j]];
        }
    }

    // update this chunk of r_i contribution for r_(i+1)
    for (i = my_nodestart; i < my_nodeend && i < nodecount; ++i) {
        my_contribution[i - my_nodestart] = my_r[i - my_nodestart] / nodehead[i].num_out_links * DAMPING_FACTOR;
    }

    // gather all chunks of r_i and update full r vector
    MPI_Gather(my_r, nodes_per_process, MPI_DOUBLE, 
                    r, nodes_per_process, MPI_DOUBLE, 
                    0, MPI_COMM_WORLD);
    // gather all chunks of r_i contribution and update full contribution vector
    MPI_Allgather(my_contribution, nodes_per_process, MPI_DOUBLE, 
                contribution, nodes_per_process, MPI_DOUBLE, 
                MPI_COMM_WORLD);
}

int main(int argc, char* argv[]) {

    // Setup each process and then start timing
    if (init(argc, argv)) return 1;
    GET_TIME(start);

    do {
        // prep last_r and r for this iteration
        if (my_rank == 0) {
            ++iterationcount;
            vec_cp(r, last_r, nodecount);
        }

        // perform the iteration chunk
        iteration();
        
        // calculate error for this iteration
        if (my_rank == 0) {
            continue_flag = (rel_error(r, last_r, nodecount) >= EPSILON);
            // printf("Iteration %d - Error: %f\n", iterationcount, rel_error(r, last_r, nodecount));
        }
        MPI_Bcast(&continue_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } while (continue_flag);

    GET_TIME(end);

    // Save output if master process
    if (my_rank == 0) {
        printf("Program converged at %d th iteration - Elapsed time %f - Process %d out of %d\n", iterationcount, end-start, my_rank + 1, processes);
        Lab4_saveoutput(r, nodecount, end-start);

        // Cleanup    
        MPI_Finalize();
        node_destroy(nodehead, nodecount);
        free(contribution);
        free(my_contribution);
        free(r);
        free(last_r);
        free(my_r);
    }
   
    return 0;
}