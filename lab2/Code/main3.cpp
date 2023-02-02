#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include"common.h"
#include"timer.h"

char **theArray;
pthread_rwlock_t rwlock;
// for timers
double* times = new double[COM_NUM_REQUEST];


void *ServerEcho(void *args)
{
    int *arrArgs = (int*) args;
    int rank = arrArgs[1];
    int clientFileDescriptor = arrArgs[0];
    char msg[COM_BUFF_SIZE];
    ClientRequest *req = new ClientRequest;
    double start; double end;

    // read and parse the message
    read(clientFileDescriptor, msg, COM_BUFF_SIZE);
    if (ParseMsg(msg, req) == 1) {
        printf("Could not parse client message.\n");
        return nullptr;
    }

    // start timer
    GET_TIME(start);

    // if the request is write
    if (!req->is_read) {
        // protect the critical section and write
        pthread_rwlock_wrlock(&rwlock);
        setContent(req->msg, req->pos, theArray);
        pthread_rwlock_unlock(&rwlock);
    }

    // get the message from the position
    pthread_rwlock_rdlock(&rwlock);
    getContent(req->msg, req->pos, theArray);
    pthread_rwlock_unlock(&rwlock);

    GET_TIME(end);

    times[rank] = end-start;

    // write it back to client
    write(clientFileDescriptor, req->msg, COM_BUFF_SIZE);

    // finish by closing the descriptor, freeing the arg and deleting the ClientRequest
    close(clientFileDescriptor);
    free(args);
    delete req;

    pthread_exit(NULL);
    return NULL;
}


int main(int argc, char* argv[])
{
    // get our command line args
    if (argc != 4) {
        printf("Error: 3 command line arguments are required.\n"); 
        return 1;
    }
    int ARR_LEN = atoi(argv[1]);
    char *IP = argv[2];
    const int PORT = atoi(argv[3]);

    // initialize an array
    initializeArray(&ARR_LEN, &theArray);

    // initialize our rw lock
    pthread_rwlock_init(&rwlock, NULL);

    // connect the server
    struct sockaddr_in sock_var;
    int serverFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
    int clientFileDescriptor;
    int i;
    pthread_t t[COM_NUM_REQUEST];

    sock_var.sin_addr.s_addr=inet_addr(IP);
    sock_var.sin_port=PORT;
    sock_var.sin_family=AF_INET;
    if(bind(serverFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0)
    {
        printf("socket has been created\n");
        listen(serverFileDescriptor, COM_NUM_REQUEST); 
        while(1)        //loop infinity
        {
            for(i=0;i<COM_NUM_REQUEST;i++)
            {
                clientFileDescriptor = accept(serverFileDescriptor,NULL,NULL);
                // printf("Connected to client %d\n", clientFileDescriptor);
                int *arg = new int[2];
                arg[0] = clientFileDescriptor;
                arg[1] = i;
                pthread_create(&t[i], NULL, ServerEcho, (void*)arg);
            }
            for (i=0;i<COM_NUM_REQUEST;i++){
                pthread_join(t[i],NULL);
            }
            saveTimes(times, COM_NUM_REQUEST);
            delete times;
            times = new double[COM_NUM_REQUEST];

        }
        close(serverFileDescriptor);
    }
    else{
        printf("socket creation failed\n");
    }
    return 0;
}