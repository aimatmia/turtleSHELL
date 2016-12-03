#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>

#include "t_rex.h"

int main(){
	while(1){
		int fdin, fdout;
		fdin = fdout = -1;
		char buf[256];
		prompt();
		readin(buf);        
		decisonmaker(buf);
	}
	return 0;
}
