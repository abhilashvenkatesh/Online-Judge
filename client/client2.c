
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

#define PORT 3050
int main(int argc,char **argv)
{   pid_t pid;
    int client_fd, new_socket, valread,file,choice;
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
    printf("list of questions:\n");
    printf("1)Alice and Bob\n");
    printf("2)Simple array sum\n");
    printf("3)Simple addition\n");
    
    printf("4)Plus or minus  \n");
    printf("Select your choice\n");
    scanf("%d",&choice);
    char Choice[5];
    bzero(&Choice,sizeof(Choice));
    sprintf(Choice,"%d",choice);
    //printf("%s\n",Choice);
    switch(choice)
    {
        case 1: write(client_fd,Choice,sizeof(Choice));
                break;
        case 2: write(client_fd,Choice,sizeof(Choice));
                break;
        case 3: write(client_fd,Choice,sizeof(Choice));
                break;
        case 4: write(client_fd,Choice,sizeof(Choice));
                break;

    }
    int qtxt_d=creat("q.txt",0666);
    bzero(&buffer,sizeof(buffer));
    while (read(client_fd,buffer,80)>0)
    {    if(strcmp(buffer,"$")==0)
            break;
        write(qtxt_d,buffer,strlen(buffer));
        bzero(&buffer,sizeof(buffer));
    }
     
    if((pid=fork())==0)
    { file=creat("add.c",0666);
      execlp("/bin/bash","bash","ki","add.c","q.txt",NULL);
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
        bzero(&recvi,sizeof(recvi));
        while(read(client_fd,recvi,80)!=0)
           {printf("%s",recvi);
             bzero(&recvi,sizeof(recvi));}   

    close(client_fd);
}

