#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

int cd(char *pth){
    int BUFFERSIZE=50;
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

int main() {
  while(1){
    char buf[50];
    printf("> ");
    //  printf("Enter commands separated by a single space: "); 

    fgets(buf, sizeof(buf), stdin);
    char * bufadd = buf;
    bufadd = strsep(&bufadd, "\n");
    char *command[20];

    int i=0;
    int first=1;
    int out=0;
    char *rest = buf;
   // *strchr(buf, "\n") = NULL;
    char  *sin, *sout;
    char  *fin, *fout;
    int fdin=-1;
    int fdout=-1;
    
    while ( (sin=strchr(rest, '<')) != NULL 
      ||  (sout=strchr(rest, '>')) != NULL ) {     
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
        if (sin != NULL && sout!=NULL)
            bufadd = strsep(&rest , "<>");         
        if (!first && sin != NULL && sout==NULL){
        // > < 
            fout = trim(strsep(&rest , "<"));
            fin = trim(rest);
        }
        // < >
        if ( !first && sin == NULL && sout!=NULL){
            fin = strsep(&rest , ">");
            fout = rest;            
        }
        first = 0;

       if (fin != NULL) {
            fdin = open(fin, O_RDONLY);
       }
       if (fout != NULL) {
            fdout = open(fout, O_CREAT, 0666);
            close(fdout);
            fdout = open(fout, O_WRONLY);
       }       
    } 
   }
}  
    
void exec(char* bufadd, char* command, int fdin, int fdout){
    bufadd = trim(bufadd);
    int saved_stdout;
    int saved_stdin;
    if (!(strcmp(command[0],"exit"))){
      exit(0);
    }
    char directories[30];
    if (!(strcmp(command[0],"cd"))){
      char* location = strchr(command[1], '\n');
      strcpy(directories, command[1]);
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

        int pid = fork();
        if (pid == 0)
           execvp( command[0], command );
        else
           wait(NULL);
        dup2(saved_stdout, 1);
        dup2(saved_stdin, 0);
   }
  }
}
