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

int cd(char *pth){
    char path[256];
    strcpy(path,pth);

    char cwd[256];
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

char* deblank(char* input)                                         
{
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)          
    {
        if (input[i]!=' ')                           
            output[j]=input[i];                     
        else
            j--;                                     
    }
    output[j]=0;
    return output;
}

void trim(char **str){
//printf("in trim %s\n", *str);
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return;

  // Trim trailing space
  end = *str + strlen(*str) - 1;
  while(end > *str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

}

/*
Readin modifies a char * array and fills it with the input from fgets
*/
void readin(char * buf){
  fgets(buf, 256, stdin); 
  *(strchr(buf, '\n')) = '\0';
}

void decisonmaker(char * buf){
  char * cmd[20];
  char * semi = NULL;
  int i = 0;
  buf = deblank(buf);

  if (!(strcmp(buf,"exit")))
    exit(0);

  if (strchr(buf, ';')) {
      char* semi = (char *)malloc(256);
      while ( semi = strsep(&buf, ";") ) {
         deblank(semi);
         for (i=0; cmd[i] = strsep(&semi, " "); i++);
         cmd[i] = 0;

         char * redir = (char *)malloc(256);
         exec(cmd, -1, -1);
       }
  }
  
  if (strchr(buf, '>') || strchr(buf, '<'))  
     redirect(buf);

  if(strchr(buf, '|'))
     peterpiper(buf);

  else {
      
      //just one command
      for (i=0; cmd[i] = strsep(&buf, " "); i++);
      //printf("cmd - %s\n", cmd[0]); 
      cmd[i] = 0;
      exec(cmd, -1, -1);
  
      if (semi !=NULL)
          free(semi);
  }
}

void redirect(char * buf){

}

void peterpiper(char * buf){
  //buf is already decisonmakerd. wake up amy.
  char * p = (char *)malloc(256);
  int stat = 0;
  int fd[2];
  int stdin = dup(STDIN_FILENO);

  p = strsep(&buf, "|"); //separate the statements
  p = deblank(p);
  buf = deblank(buf);

  pipe(fd);
  int pid = fork(); //giving birth

  //it's a child!
  if (!pid){
    close(fd[0]); //close the read part bc we want the child to write its output so that we can use it
    dup2(fd[1],STDOUT_FILENO); //swap stdout with write 
    decisonmaker(p); //send it to decisonmaker to exec
    exit(0); //bye bye child
  }

  //parenting sucks
  else{
    wait(&stat);
    close(fd[1]); //dont need write if its parent. u want READ
    dup2(fd[0], STDIN_FILENO); //replaces stdin with pipe read
    decisonmaker(buf); //execute
    dup2(stdin, STDIN_FILENO); //same thing but will return STDIN
  }
}


/*    int i=0;
    int first=1;
    char *rest = buf;
    char  *sin, *sout;
    char  *fin, *fout;
    int fdin=-1;
    int fdout=-1;
    
    while (1) { 
       sin=strchr(rest, '<');
       sout=strchr(rest, '>');
       printf("!!in=%s, out=%s\n", sin, sout);
       if (sin == NULL &&  sout == NULL )
           break; 
    
      //contains "<" or ">" 
        printf("rest=%s\n", bufadd );
        if (first && sin != NULL && sout==NULL){
        // only input
            bufadd = strsep(&rest , "<");
            fin = deblank(rest);
printf("1\n");
        }
        if (first && sin == NULL && sout!=NULL){
        // only output
            bufadd = strsep(&rest , ">");
            fout = deblank(rest);
printf("2\n");   
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
               rest = deblank(r1);
            }
            else {
               bufadd = buf2;
               rest = deblank(r2); 
            }
            printf("buf=%s, rest=%s\n", bufadd, rest);
        }         
        if (!first && sin != NULL && sout==NULL){
        // > < 
            fout = deblank(strsep(&rest , "<"));
            fin = deblank(rest);
printf("4\n");   
        }
        // < >
        if ( !first && sin == NULL && sout!=NULL){
            fin = strsep(&rest , ">");
            fout = deblank(rest);  
printf("5\n");             
        }
        first = 0;
  } 
  printf("in=%s, out=%s\n", fin, fout);
  if (fin != NULL) 
       fdin = open(fin, O_RDONLY);
  if (fout != NULL) {
       fdout = open(fout, O_CREAT, 0666);
       close(fdout);
       fdout = open(fout, O_WRONLY);
  }       
  deblank(bufadd);
  exec(bufadd, fdin, fdout);
    
  }
*/


void exec(char** cmd, int fdin, int fdout){
    int saved_stdout = 1;
    int saved_stdin = 0;
    char directories[30];
    //printf("cmd %s- %s\n", cmd[0], cmd);

    if (!(strcmp(cmd[0],"cd"))){
      char* location = strchr(cmd[1], '\n');
      strcpy(directories, cmd[1]);
      cd(directories);
    }   
    
    else {
      if(fdin != -1){
          saved_stdin = dup(0);
          dup2(fdin, 0);
      }
      if(fdout!= -1) {
          saved_stdout = dup(1);
          dup2(fdout, 1);
      }
     
      int tpid;
      int stat=0;
      int chpid = fork();
      if (chpid == 0) 
         execvp( cmd[0], cmd );
      else {
         do { 
           tpid = wait(&stat);
           //printf("id %d\n", tpid);
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


// int main(){
//   while(1){
//     int fdin, fdout;
//     fdin = fdout = -1;
//     char buf[256];
//     prompt();
//     readin(buf);        
//     decisonmaker(buf);
//   }
//   return 0;
// }