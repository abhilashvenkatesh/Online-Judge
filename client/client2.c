#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 1250
int main(int argc,char **argv)
{   pid_t pid;
    int client_fd, new_socket, valread,file;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
     char filename[80],recvi[80],buffer[80];
      
    // Creating socket file descriptor
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
     bzero(&address,addrlen);
      
    // Forcefully attaching socket to the port 8080
    address.sin_family = AF_INET;   
    //address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    inet_pton(AF_INET,argv[1],&address.sin_addr);
    connect(client_fd,(struct sockaddr*)& address,addrlen);

    if((pid=fork())==0)
    { file=creat("add.c",0666);
      execlp("/usr/bin/vi","vi","add.c",NULL);         //vi editor
    }
    else
        { wait(NULL);
        //sleep(2);
    }

            
    FILE *Compile_d=fopen("add.c","r");
    if(Compile_d==NULL)
        perror("file doesn't exist");
    
   
    bzero(&buffer,sizeof(buffer));
    while(fgets(buffer,80,Compile_d)!=NULL)       // code submission
        { //puts(buffer);
            write(client_fd,buffer,sizeof(buffer)); 
            bzero(&buffer,sizeof(buffer))  ;
        }
        write(client_fd,"$",1);         
        while(read(client_fd,recvi,80)!=0)              // recieve message-compile error/correct answer/wrong answer
           {printf("%s",recvi);
             bzero(&recvi,sizeof(recvi));}   

    close(client_fd);
}

