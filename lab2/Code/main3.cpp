#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include"common.h"

char **theArray;

void *ServerEcho(void *args)
{
    int clientFileDescriptor = (intptr_t)args;
    char msg[COM_BUFF_SIZE];
    ClientRequest *req = nullptr;

    // read and parse the message
    read(clientFileDescriptor,msg,COM_BUFF_SIZE);
    ParseMsg(msg, req);

    if (req->is_read) {
        getContent(req->msg, req->pos, theArray);
    }
    else {
        setContent(req->msg, req->pos, theArray);
    }
    // printf("reading from client:%s\n",msg);
    // write(clientFileDescriptor, msg, COM_BUFF_SIZE);
    close(clientFileDescriptor);
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
        listen(serverFileDescriptor,2000); 
        while(1)        //loop infinity
        {
            for(i=0;i<COM_NUM_REQUEST;i++)
            {
                clientFileDescriptor=accept(serverFileDescriptor,NULL,NULL);
                printf("Connected to client %d\n",clientFileDescriptor);
                pthread_create(&t[i],NULL,ServerEcho,(void *)(long)clientFileDescriptor);
            }
        }
        close(serverFileDescriptor);
    }
    else{
        printf("socket creation failed\n");
    }
    return 0;
}