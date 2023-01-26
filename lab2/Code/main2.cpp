#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<string>

char** theArray
pthread_mutex_t* mutexes;

void *ServerEcho(void *args)
{
    char* clientFileDescriptor = (intptr) args;
    char str[20];
    ClientRequest *request = nullptr;

    read(clientFileDescriptor,str,20);
    ParseMsg(str, str);

    if (request->is_read){
        getContent(request->msg, request->pos, theArray);
    }
    else{
        setContent(request->msg, request->pos, theArray);
    }

    printf("reading from client:%s\n",str);
    // write(clientFileDescriptor,str,20);
    close(clientFileDescriptor);
    return NULL;


}


int main(int argc, char* argv[])
{
    // Get command line args
    int arraySize = atoi(argv[1])
    char* ip = argv[2]
    const int port = atoi(argv[3])

    // Instantiate array
    theArray = initializeArray(&arraySize, &thearray)
    mutexes = new pthread_mutex_t[arraySize]

    struct sockaddr_in sock_var;
    int serverFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
    int clientFileDescriptor;
    int i;
    pthread_t t[20];

    sock_var.sin_addr.s_addr=inet_addr(ip);
    sock_var.sin_port=port;
    sock_var.sin_family=AF_INET;
    if(bind(serverFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0)
    {
        printf("socket has been created\n");
        listen(serverFileDescriptor,2000); 
        while(1)        //loop infinity
        {
            for(i=0;i<20;i++)      //can support 20 clients at a time
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