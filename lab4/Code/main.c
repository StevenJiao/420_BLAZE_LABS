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

int main(int argc, char* argv[])
{
    struct node *nodehead;
    int nodecount;
    double *r, *r_pre, *contribution;
    int i, j;
    double damp_const;
    int iterationcount = 0;
    double start, end;
    FILE *ip;

    // Read the input file
    if ((ip = fopen("data_input_meta","r")) == NULL) {
        printf("Error opening the data_input_meta file.\n");
        return 254;
    }
    fscanf(ip, "%d\n", &nodecount);
    fclose(ip);
    
    // Initialize the graph
    if (node_init(&nodehead, 0, nodecount)) return 254;


    // Initialize r vectors, r_i(0) = 1/N
    r = malloc(nodecount * sizeof(double));
    r_pre = malloc(nodecount * sizeof(double));
    for ( i = 0; i < nodecount; ++i)
        r[i] = 1.0 / nodecount;
    
    // Calculate initial contribution of all nodes (D_i) with an edge to each node i
    contribution = malloc(nodecount * sizeof(double));
    for ( i = 0; i < nodecount; ++i)
        contribution[i] = r[i] / nodehead[i].num_out_links * DAMPING_FACTOR;
    damp_const = (1.0 - DAMPING_FACTOR) / nodecount;

    // Core iteration
    GET_TIME(start);
    do {
        ++iterationcount;
        vec_cp(r, r_pre, nodecount);
        
        // Update r vector for this t (iteration)
        for ( i = 0; i < nodecount; ++i){
            r[i] = 0;
            for ( j = 0; j < nodehead[i].num_in_links; ++j)
                r[i] += contribution[nodehead[i].inlinks[j]];
            r[i] += damp_const;
        }
        // Update contribution of all nodes (D_i) with an edge to each node i, used
        // in next iteration for vector r(t+1)
        for ( i=0; i<nodecount; ++i){
            contribution[i] = r[i] / nodehead[i].num_out_links * DAMPING_FACTOR;
        }
    } while(rel_error(r, r_pre, nodecount) >= EPSILON);
    
    GET_TIME(end);
    printf("Program converged at %d th iteration.\nElapsed time %f.\n", iterationcount, end-start);

    Lab4_saveoutput(r, nodecount, end-start);

    // post processing
    node_destroy(nodehead, nodecount);
    free(contribution);

    return 0;
}