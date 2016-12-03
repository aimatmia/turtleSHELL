#ifndef T_REX
#define T_REX

char *trim(char *str);
int cd(char *pth);
void exec_1com(char* buf, int fdin, int fdout);
void exec(char** cmd, int fdin, int fdout);
void parse(char * buf);
void peterpiper(char * buf);
void prompt();
void readin(char * buf);
void redirect(char * bufadd);

#endif
