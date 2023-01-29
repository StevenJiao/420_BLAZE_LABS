#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<string>
#include<cstdint>
#include<stdint.h>
#include<cstdlib>
#include<iostream>
#include"common.h"

char **theArray;
pthread_mutex_t *locks;


void *ServerEcho(void *args)
{
    int clientFileDescriptor = *((int*)args);
    char str[COM_BUFF_SIZE];
    ClientRequest *request = new ClientRequest;

    // read and parse message
    read(clientFileDescriptor,str,COM_BUFF_SIZE);
    ParseMsg(str, request);

    // Perform read ops
    if (!request->is_read){

        // Open string-specific mutex, write to string
        pthread_mutex_lock(&locks[request->pos]);
        setContent(request->msg, request->pos, theArray);
        pthread_mutex_unlock(&locks[request->pos]);
    }

    // Lock array mutex for read
    pthread_mutex_lock(&locks[request->pos]);
    getContent(request->msg, request->pos, theArray);
    pthread_mutex_unlock(&locks[request->pos]);

    // Send data
    write(clientFileDescriptor, request->msg, COM_BUFF_SIZE);
    close(clientFileDescriptor);
    
    // Final cleanup ops
    free(args);
    delete request;
    pthread_exit(NULL);    
    return NULL;
}


int main(int argc, char* argv[])
{
    std::cout << "0";

    // Get command line args
    int arraySize = atoi(argv[1]);
    char* ip = argv[2];
    const int port = atoi(argv[3]);

    // Instantiate array
    initializeArray(&arraySize, &theArray);

    // Initailize mutex for each string in array
    locks = new pthread_mutex_t[arraySize];
    for (int i=0; i<arraySize; i++) {
        if (pthread_mutex_init(&locks[i], NULL) != 0) {
            printf("Mutex %i could not be made\n", i);
            return 1;
        }
    }

    struct sockaddr_in sock_var;
    int serverFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
    int clientFileDescriptor;
    int i;
    pthread_t t[COM_NUM_REQUEST];

    sock_var.sin_addr.s_addr=inet_addr(ip);
    sock_var.sin_port=port;
    sock_var.sin_family=AF_INET;
    if(bind(serverFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0)
    {
        printf("socket has been created\n");
        listen(serverFileDescriptor,2000); 
        while(1)        //loop infinity
        {
            for(i=0;i<COM_NUM_REQUEST;i++)      //can support COM_NUM_REQUEST clients at a time
            {
                clientFileDescriptor=accept(serverFileDescriptor,NULL,NULL);
                // printf("Connected to client %d\n",clientFileDescriptor);
                pthread_create(&t[i],NULL,ServerEcho,(void *)(long)clientFileDescriptor);
            }

            // Wait for the threads to terminate
            for (i=0;i<COM_NUM_REQUEST;i++){
                pthread_join(t[i],NULL);
            }
        }
        close(serverFileDescriptor);
    }
    else {
        printf("socket creation failed\n");
    }
    return 0;
}