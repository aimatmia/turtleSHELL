#ifndef T_REX
#define T_REX

int cd(char *pth);
void trim(char **str);
void readin(char *buf);
void parse(char * buf);
void exec(char ** cmd, int fdin, int fdout);

#endif
