#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>

#include "t_rex.h"

/**********************************************************************
PROMPT: gets current directory and user. prints out the appropriate 
command line prompt depending on whether the hostname is null or not.  
Takes as input: none  
Returns: none 
***********************************************************************/  
void prompt(){
  char getcd[50];
  char hostname[50];
  if (gethostname(hostname, sizeof(hostname)) == -1)
    printf("%s ", getcwd(getcd, sizeof(getcd)));
  else
    printf("%s %s$ ", getcwd(getcd, sizeof(getcd)), getenv("USER"));
}

/**********************************************************************
CD: takes in a path string, modifies the string if needed and changes 
the directory to the path string.  
Takes as input: input string (path)  
Returns: int (returns 0 upon completion) 
***********************************************************************/
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

/**********************************************************************
DEBLANK: removes extra white space from around a given string  
Takes as input: input string  
Returns: "trimmed" output string 
**********************************************************************/
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

char * trim(char **str){
// //printf("in trim %s\n", *str);
//   char *end;

//   // Trim leading space
//   while(isspace(*str)) str++;

//   if(*str == 0)  // All spaces?
//     return "";

//   // Trim trailing space
//   end = *str + strlen(*str) - 1;
//   while(end > *str && isspace((unsigned char)*end)) end--;

//   // Write new null terminator
//   *(end+1) = 0;

//   return str;
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

/*********************************************************************
READIN: reads (fgets) input from the terminal into the string buffer 
Takes as input: input string buffer 
Returns: none 
*********************************************************************/
void readin(char * buf){
  fgets(buf, 256, stdin); 
  *(strchr(buf, '\n')) = '\0';
}

/********************************************************************
DECISIONMAKER: takes in string and after searching for certain 
characters such as '<', '>', or '|' determines the appropriate action 
to take. calls on exec.  
Takes as input: input string  
Returns: none 
********************************************************************/
void decisonmaker(char * buf){
  char * cmd[20];
  char* semi = (char *)malloc(256);
  int i = 0;
  // buf = trim(buf);
  //buf = deblank(buf);
  if (!(strcmp(buf,"exit")))
    exit(0);

  else if (strchr(buf, ';')) {
    printf("hello its me\n");
        while (semi = strsep(&buf, ";") ) {
           //deblank(semi);
           //printf("semi: %s\n", semi);
           //printf("buf: %s\n", buf);
           // for (i=0; cmd[i] = strsep(&semi, " "); i++);
           // cmd[i] = 0;
           // exec(cmd, -1, -1);
           semi= trim(semi);
           exec_1com(semi, NULL, NULL);
         }
  }

  else if (strchr(buf, '<')){
    //printf("hiiii\n");
    redirectL(buf);
  }

  else if (strstr(buf, ">>")){
    redirectRA(buf);
  }
  else if (strchr(buf, '>')){
    redirectR(buf);
  }

  else if(strchr(buf, '|'))
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

/*******************************************************************
REDIRECTR: Splits input string on ">", conducts the
appropriate redirection (including opening files), and executes 
Takes as input: input string  
Returns: none 
********************************************************************/
void redirectR(char * buf){
  char * p = (char *)malloc(256);
  p = strsep(&buf, ">");
  p = deblank(p);
  buf = deblank(buf);
  int stdout = dup(STDOUT_FILENO);
  trim(&buf);
  int op = open(buf, O_WRONLY | O_TRUNC | O_CREAT, 0644);
  dup2(op, STDOUT_FILENO);
  //printf("buffy: %s\n", buf);
  decisonmaker(p);
  dup2(stdout, STDOUT_FILENO);
  close(op);
}

void redirectRA(char * buf){
  char * p = (char *)malloc(256);
  p = strsep(&buf, ">>");
  p = deblank(p);
  buf = deblank(buf);
  buf += 1;
  int stdout = dup(STDOUT_FILENO);
  trim(&buf);
  int op = open(buf, O_APPEND | O_WRONLY | O_CREAT, 0644);
  dup2(op, STDOUT_FILENO);
  //printf("buffy: %s\n", buf);
  decisonmaker(p);
  dup2(stdout, STDOUT_FILENO);
  close(op);
}

/*******************************************************************
REDIRECTL: Splits input string on "<", conducts the
appropriate redirection (including opening files), and executes
Takes as input: input string  
Returns: none 
********************************************************************/
void redirectL(char * buf){
  char * p = (char *)malloc(256);
  p = strsep(&buf, "<");
  p = trim(p);
  buf = trim(buf);
  int stdin = dup(STDIN_FILENO);
  trim(&buf);
  int op = open(buf, O_RDONLY, 0666);
  dup2(op, STDIN_FILENO);
  decisonmaker(p);
  dup2(stdin, STDIN_FILENO);
  close(op);
}


/*******************************************************************
PETERPIPER: separates the string by '|', adjusts redirection, and 
executes after forking  
Takes as input: input string  
Returns: none 
********************************************************************/
void peterpiper(char * buf){
  //buf is already decisonmakerd. wake up amy.
  char * p = (char *)malloc(256);
  int stat = 0;
  int fd[2];
  int stdin = dup(STDIN_FILENO);

  p = strsep(&buf, "|"); //separate the statements
  // p = deblank(p);
  // buf = deblank(buf);
  p = trim(p);
  buf = trim(buf);

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

/*******************************************************************
EXEC: deals with cd, exit, and calls on execvp to execute
commands
Takes as input: takes in pointer to array of pointers, two ints 
(input and output file descriptors)  
Returns: none 
********************************************************************/
void exec(char** cmd, int fdin, int fdout){
    int saved_stdout = 1;
    int saved_stdin = 0;
    char directories[30];
    //printf("cmd %s- %s\n", cmd[0], cmd);
    if (!(strcmp(cmd[0],"exit")))
      exit(0);

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
/*******************************************************************
EXEC_1COM: deals with exec cases where ";" is combined with redirection
Takes as input: takes in pointer to array of pointers, two ints 
(input and output file descriptors)  
Returns: none 
********************************************************************/
void exec_1com(char* buf, char *fin, char *fout){
    char * cmd[20];
    int i; 
    char bufsave[50];
    strcpy(bufsave, buf);
    for (i=0; cmd[i] = strsep(&buf, " "); i++);
    cmd[i] = 0;

    //char * redir = (char *)malloc(256);
    printf("bufsave %s\n", bufsave);
    if (strchr(bufsave, '>'))  
         redirectR(bufsave);
    else if (strchr(bufsave, '<'))
      redirectL(bufsave);
    else
         exec(cmd, fin, fout); 
}
