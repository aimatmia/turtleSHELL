#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h> 

int readin(){
  char store[256];
  printf("Enter command: \n");
  fgets(store, 256, stdin);
  store[strlen(store) - 1] = 0;
  char * line = store;
  char * params[10];
  int i = 0;

  //while (kline){
  //params[n]=strsep(&line, " ");  
  //i++;}
  //use strchr to find terminating null
  *strchr(store, "\n") = NULL;
  for (i; params[i] = strsep(&line, " "); i++);
  //params[i] = 0;
  execvp(params[0], params);
  return 0;
}

int main(){
  readin();
}
  
