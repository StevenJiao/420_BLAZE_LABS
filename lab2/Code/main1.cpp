#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>

// For intptr_t cast
#include <cstdint>
#include <stdint.h>

// For atoi
#include <cstdlib>

#include "common.h"
#include "timer.h"

char** arr;
pthread_mutex_t lock;
int client_fds[COM_NUM_REQUEST];

// for timers
double* times = new double[COM_NUM_REQUEST];

void *ServerThread(void *args)
{
    int *arrArgs = (int*) args;
    int rank = arrArgs[1];
    intptr_t clientFileDescriptor = (intptr_t) arrArgs[0];

    // Timer variables
    double start, end;

    // Allocate and read the max buffer size
    char str[COM_BUFF_SIZE];
    char req_str[COM_BUFF_SIZE];
    read(clientFileDescriptor, str, COM_BUFF_SIZE);

    // Parse the request
    ClientRequest* request = new ClientRequest;
    if (ParseMsg(str, request) == 1) {
        printf("Could not parse client message.\n");
        return nullptr;
    }
    // printf("Received: is_read: %d, pos: %d msg: %s\n", (*request).is_read, (*request).pos, (*request).msg);

    // start timer
    GET_TIME(start);

    // Write if request said to, and always read after
    if (!(*request).is_read) {
        // Lock array mutex for write
        pthread_mutex_lock(&lock);
        setContent((*request).msg, (*request).pos, arr);
        pthread_mutex_unlock(&lock);
    }
    
    // Lock array mutex for read
    pthread_mutex_lock(&lock);
    getContent(req_str, (*request).pos, arr);
    pthread_mutex_unlock(&lock);

    // Write times
    GET_TIME(end);
    times[rank] = end-start;

    // Respond with value we read
    write(clientFileDescriptor, req_str, COM_BUFF_SIZE);
    close(clientFileDescriptor);

    // Cleanup
    delete request;
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char* argv[])
{

    // Ensure we got enough arguements
    if (argc != 4) {
        printf("Error: 3 command line arguments are required. <array_size> <server_ip> <server_port>\n"); 
        return 1;
    }

    // Get argument values
    int arr_len = atoi(argv[1]);
    char* server_ip = argv[2];
    int server_port = atoi(argv[3]);

    // Instantiate socket
    struct sockaddr_in sock_var;
    int serverFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
    int i;
    pthread_t t[COM_NUM_REQUEST];

    // Configure socket
    sock_var.sin_addr.s_addr = inet_addr(server_ip);
    sock_var.sin_port = server_port;
    sock_var.sin_family=AF_INET;

    // Init array
    printf("Array length: %d, IP: %s, Port: %d\n", arr_len, server_ip, server_port);
    initializeArray(&arr_len, &arr);

    // Init mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex creation failed\n");
        return 1;
    }

    if(bind(serverFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0) {
        printf("socket has been created\n");
        listen(serverFileDescriptor,2000); 
        while(1) {
            // Spawn a thread for every new socket connection
            for(i=0;i<COM_NUM_REQUEST;i++) {
                client_fds[i] = accept(serverFileDescriptor,NULL,NULL);
                // printf("Connected to client %d\n",client_fds[i]);
                int *arg = new int[2];
                arg[0] = client_fds[i];
                arg[1] = i;
                pthread_create(&t[i], NULL, ServerThread, (void *)arg);
            }

            for (i=0;i<COM_NUM_REQUEST;i++){
                pthread_join(t[i],NULL);
            }

            // Record times
            saveTimes(times, COM_NUM_REQUEST, "server_output_time_aggregated");
            delete[] times;
            times = new double[COM_NUM_REQUEST];
        }

        // Cleanup
        close(serverFileDescriptor);
        pthread_mutex_destroy(&lock);
    }
    else{
        printf("socket creation failed\n");
    }

    return 0;
}