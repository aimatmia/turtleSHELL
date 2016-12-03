#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>

#include "t_rex.h"

/*
T-REX:
Trim-Readin-Execute
*/

/*
Trim gets rid of the trailing and front spaces
*/
void prompt(){
  char getcd[50];
  char hostname[50];
  if (gethostname(hostname, sizeof(hostname)) == -1)
    printf("%s ", getcwd(getcd, sizeof(getcd)));
  else
    printf("%s %s$ ", getcwd(getcd, sizeof(getcd)), getenv("USER"));
}

char *trim(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

int cd(char *pth){
    int BUFFERSIZE=256;
    char path[BUFFERSIZE];
    strcpy(path,pth);

    char cwd[BUFFERSIZE];
    if(pth[0] != '/')
    {// true for the dir in cwd
        getcwd(cwd,sizeof(cwd));
        strcat(cwd,"/");
        strcat(cwd,path);
        chdir(cwd);
    }else{//true for dir w.r.t. /
        chdir(pth);
    }
    return 0;
}

//Readin modifies a char * array and fills it with the input from //fgets
void readin(char * buf){
	fgets(buf, 256, stdin);	
	*(strchr(buf, '\n')) = '\0';
}

/*
Parse trims the buf, getting rid of any unneccessary space between and after and then modifies
the given char ** cmd, filling it with the input from buf
*/
void parse(char * buf){
  char * semi = NULL;
  int i = 0;
  buf= trim(buf);
  //if multiple commands
  if (strchr(buf, ';')) {
      char* semi = (char *)malloc(256);
      while ( semi = strsep(&buf, ";") ) {       
         semi= trim(semi);
         exec_1com(semi, -1, -1);
       }
       if (semi !=NULL)
          free(semi);
  }
  else {
       //just one command

       exec_1com(buf, -1, -1);
         }
}

void exec_1com(char* buf, int fdin, int fdout){
    char * cmd[20];
    int i; 
    char bufsave[50];
    strcpy(bufsave, buf);
    for (i=0; cmd[i] = strsep(&buf, " "); i++);
    
    cmd[i] = '\0';
   
    if (strchr(bufsave, '<') || strchr(bufsave, '>') )  
         redirect(bufsave);
    else if (strchr(bufsave, '|') )  
         peterpiper(bufsave);
    else
         exec(cmd, fdin, fdout); 
}


void peterpiper(char * buf){
  char * exe1 = (char *)malloc(256);
  char* exe2;
  int stat = 0;
  int fd[2];
  int fdin = dup(STDIN_FILENO);

  exe1 = trim( strsep(&buf, "|") ); //separate the statements
  exe2 = trim(buf);

  pipe(fd);
  int pid = fork(); //giving birth

  //it's a child!
  if (!pid){
    close(fd[0]); //close the read part bc we want the child to write its output so that we can use it
    dup2(fd[1],STDOUT_FILENO); //swap stdout with write 
    exec_1com(exe1, -1, fd[1] ); //send it to exec
    exit(0); //bye bye child
  }
//parenting sucks
  else{
    wait(&stat);
    close(fd[1]); //dont need write if its parent. u want READ
    dup2(fd[0], STDIN_FILENO); //replaces stdin with pipe read
    exec_1com(exe2, fd[0], -1 ); //execute
    dup2(fdin, STDIN_FILENO); //same thing but will return STDIN
  }
}

void redirect(char * bufadd) {
    int i=0;
    int first=1;
    char *rest = bufadd;
    char  *sin=NULL; 
    char  *sout=NULL;
    char  *fin=NULL;
    char *fout=NULL;
    int fdin=-1;
    int fdout=-1;
    
    while (1) { 
       sin=strchr(rest, '<');
       sout=strchr(rest, '>');

       if (sin == NULL &&  sout == NULL )
           break; 
    
      //contains "<" or ">" 
        if (first && sin != NULL && sout==NULL){
        // only input
            bufadd = strsep(&rest , "<");
            fin = trim(rest);
        }
        if (first && sin == NULL && sout!=NULL){
        // only output
            bufadd = strsep(&rest , ">");
            fout = trim(rest);
        }
        if (sin != NULL && sout!=NULL) {
            char *buf1, *buf2;
            char* r1, *r2;
            r1 = malloc(30*sizeof(char));
            r2 = malloc(30*sizeof(char));
            strcpy( r1, rest);
            strcpy( r2, rest);
            buf1 = strsep(&r1 , "<");
            buf2 = strsep(&r2 , ">");
            if ( strlen(r1) > strlen(r2) ) {
               bufadd = buf1;
               rest = trim(r1);
            }
            else {
               bufadd = buf2;
               rest = trim(r2); 
            }
        }         
        if (!first && sin != NULL && sout==NULL){
        // > < 
            fout = trim(strsep(&rest , "<"));
            fin = trim(rest);
        }
        // < >
        if ( !first && sin == NULL && sout!=NULL){
            fin = trim( strsep(&rest , ">"));
            fout = trim(rest);  
        }
        first = 0;
  } 

  if (fin != NULL) 
      fdin = open(fin, O_RDONLY, 0666);
  if (fout != NULL) 
      fdout = open(fout, O_WRONLY | O_TRUNC | O_CREAT, 0644);

  bufadd= trim(bufadd);
  exec_1com(bufadd, fdin, fdout);
    

}


void exec(char** cmd, int fdin, int fdout){
    int saved_stdout = 1;
    int saved_stdin = 0;
    char directories[30];
            
    if (!(strcmp(cmd[0],"exit")))
      exit(0);

    if (!(strcmp(cmd[0],"cd"))){
      char* location = strchr(cmd[1], '\n');
      strcpy(directories, cmd[1]);
      cd(directories);
    }   
    
    else {
       if(fdin != -1){
       //   printf("input\n");
          saved_stdin = dup(0);
          dup2(fdin, 0);
       }
       if(fdout!= -1) {
       //   printf("output\n");
          saved_stdout = dup(1);
          dup2(fdout, 1);        
        }
     
      int i, tpid;
      int stat=0;
      int chpid = fork();
      if (chpid == 0) 
         execvp( cmd[0], cmd );
  
      else { // parent         
         do { 
           tpid = wait(&stat);
          
         } while(tpid != -1);
    
      } 
      if(fdin != -1){          
          close(fdin);
          dup2(saved_stdin, 0);

      }
      if(fdout!= -1) {
          close(fdout);
          dup2(saved_stdout, 1);  
          
      }
    }
}
