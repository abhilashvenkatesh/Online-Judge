#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 1250
int main(int argc,char **argv)
{
    int client_fd, new_socket, valread;
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
    printf("enter filename\n");
    scanf("%s",filename);   
    FILE *Compile_d=fopen(filename,"r");
    if(Compile_d==NULL)
        perror("file doesn't exist");
    
    write(client_fd,filename,80);
     
    

    while(fgets(buffer,80,Compile_d)!=NULL)       // code submission
        { // puts(buffer);
            write(client_fd,buffer,sizeof(buffer));   
        }
        write(client_fd,"$",1);
        while(read(client_fd,recvi,80)!=0)
           {printf("%s",recvi);
        }   

    close(client_fd);
}

