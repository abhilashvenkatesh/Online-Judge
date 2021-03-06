//challenges/to do- connect with different computers,concurrent server,multiple questions,naming conventions for files
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

int compile( char Compile[],char Error[],char Serve[]);
int execute(char Serve[],char Out[],char ip[]);
int check(char Out[],char op[]);
int delete( char Compile[],char Error[],char Serve[],char Out[]);
void backend(int Client_d);
char compile_file[]="CODE.c";
 char error_file[]="error.txt";
 char out_file[]="out.txt";
 char output[]="serve";
 char correct[]="correct answer\n";
 char wrong[]="wrong answer\n";
 char compile_err[]="compilation error\n";
 char stdinput[]="ip";
 char stdoutput[]="op";
 char question[]="q.txt";
 
int flag[10];
#define PORT 3050
int main(int argc,char **argv)
{   
    pid_t pid;
    int server_fd, Client_d, valread,opt=1;
    struct sockaddr_in address,clientaddr ;
    
    int addrlen = sizeof(address);
    int clienlen = sizeof(clientaddr);
    bzero(&flag,sizeof(flag));
   // printf("%d\n",flag[9]);
      
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      bzero(&address,addrlen);
      if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // Forcefully attaching socket to the port 8080
    address.sin_family = AF_INET;
    //address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed45");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd,5) < 0)
    {
        perror("listen");

        exit(EXIT_FAILURE);
    }
    
    
    while(1)
    {if ((Client_d = accept(server_fd, (struct sockaddr *)&clientaddr, 
                       (socklen_t*)&clienlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    } 
    int i=0;
    
     backend(Client_d); // code file transfer,compile,execute,
     
    }
       
}

int compile( char Compile[],char Error[],char Serve[])
{int pid;
    int Error_d=creat(Error,0666);
    char error[10];
    pid=fork();
    if(pid<0)
        exit(0);
    else if(pid==0)
    {
         dup2(Error_d,2);
         

        execlp("/usr/bin/gcc","gcc",Compile,"-o",Serve,NULL);
    }
    else
    {   
        wait(NULL);
        //printf("%d\n",pid);
        FILE *fp;
        fp=fopen(Error,"r");
        if(fp==NULL)
            perror("file doesn't exist");
        if(fgets(error,10,fp)!=NULL)
            { 
                fclose(fp);
                close(Error_d);
             return 0;
            }
        fclose(fp);
        close(Error_d);
        return 1;
    }
    
}

int execute(char Serve[],char Out[],char ip[])
{int pid;
    
    if((pid=fork())==0)
           {
            
            execlp("/bin/bash","bash","/home/server/ex",Serve,Out,ip,NULL);
            }
    else if(pid>0)
    { 

    wait(NULL);
	    
    return 0;
    }
}
int check(char Out[],char op[])
{// printf("hi8\n");
    FILE *Stdop_d,*Out_d;
    Stdop_d=fopen(op,"r");
    if(Stdop_d==NULL)
        {perror("not opening..");
          return 0;
        }
    Out_d=fopen(Out,"r");
   if(Out_d==NULL)
         {perror("not opening..");
          return 0;
        }
    char ch1,ch2;
    ch1 = getc(Stdop_d);
      ch2 = getc(Out_d);
 
    while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) {
         ch1 = getc(Stdop_d);
         ch2 = getc(Out_d);
         //printf("%c",ch2);
      }
 
      fclose(Out_d);
      fclose(Stdop_d);
      
      if (ch1 == ch2)
         return 1;
      else if (ch1 != ch2)
         return 0;
 
    return 1;

}

int delete( char Compile[],char Error[],char Serve[],char Out[])
{int pid;
     int er=creat("err.txt",0666);
    if((pid=fork())==0)
     { dup2(er,2);
     execlp("/bin/rm","rm",Error,Compile,Serve,Out,NULL);       
      }
     else
    {wait(NULL);
     return 1;
    }

    

}

void backend(int Client_d)
{int choice;
  char Choice[5];
 char buffer[80];
 bzero(&buffer,sizeof(buffer));
 read(Client_d,Choice,80);
 strcpy(buffer,"/home/server/");
 strcat(buffer,Choice);

if (chdir(buffer) <0)
         {perror("directory not changed");
 			  exit(0);
 			}
 sprintf(buffer,"%d",Client_d);
 char Compile[10];
 char Error[10];
 char Out[10];
 char Serve[10];
 strcpy(Compile,strcat(buffer,compile_file));
 sprintf(buffer,"%d",Client_d);
 strcpy(Error,strcat(buffer,error_file));
 sprintf(buffer,"%d",Client_d);
 strcpy(Out,strcat(buffer,out_file));
 sprintf(buffer,"%d",Client_d);
 strcpy(Serve,strcat(buffer,output));
  
    FILE *fp;
   
    fp=fopen(question,"r");
     bzero(&buffer,sizeof(buffer));
    while(fgets(buffer,80,fp)!=NULL)
    { write(Client_d,buffer,sizeof(buffer));
      bzero(&buffer,sizeof(buffer));
          
    }
    write(Client_d,"$",1);
    fclose(fp);
    
    bzero(&buffer,sizeof(buffer));
    int Compile_d=creat(Compile,0666);
    int flag_compiled=1;
    int Out_d=creat(Out,0666);
     int serve=creat(Serve,0666);
     close(Out_d);
    close(serve);


    // below to store submitted code in server
    while(read(Client_d,buffer,80)>0) //to avoid deadlock
        {  
            if(strcmp(buffer,"$")==0) 
            break;
            write(Compile_d,buffer,strlen(buffer));
            
            bzero(&buffer,sizeof(buffer));
            
        }
        
        bzero(&buffer,sizeof(buffer));
    
    
    if(compile(Compile,Error,Serve))
    {  char ip[10]; char op[10];
        int Correct=0,i;
       for ( i=1;i<=5;i++)
    	{  sprintf(ip,"%d",i);
           strcat(ip,stdinput);
           sprintf(op,"%d",i);
           strcat(op,stdoutput);
           
    		execute(Serve,Out,ip);  

        
                 // execute for five test cases and check with stdout
        if(check(Out,op))
            {
               Correct++;
            }
            else
            { ;
                
              break;
            } 
         }
     
         if (Correct==5)
         	write(Client_d,correct,sizeof(correct));  
         else 
         	 write(Client_d,wrong,sizeof(wrong));

       }
    else
        {    
            fp=fopen(Error,"r");
            if(fp==NULL)
                perror("can't open file");
        
        
        write(Client_d,compile_err,sizeof(compile_err));
        while(fgets(buffer,80,fp)!=NULL)
        {write(Client_d,buffer,sizeof(buffer)); //use sizeof  
          bzero(&buffer,sizeof(buffer));
        } 
        fclose(fp);
    
        }
        close(Compile_d);
        
    delete(Compile,Error,Serve,Out);
    if (chdir("/home/server/") <0)
         {perror("directory not changed");
 			  exit(0);
 		}
    
    close(Client_d);
}
