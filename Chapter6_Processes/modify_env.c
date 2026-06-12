#define _GNU_SOURCE
#include <stdlib.h>
#include "../Lib/tlpi_hdr.h"
#include<stdio.h>
extern char **environ;
int main(int argc,char *argv[]) {
	int j;
	char **ep;
	clearenv();//Erase entire environment;
	for (int i=1;i<argc;i++) if (putenv(argv[i])) errExit("putenv: %s",argv[i]);
	if(setenv("GREET","HelloWord",0)==-1) errExit("setenv");
	unsetenv("BYE");
	for (ep=environ;ep!=NULL;ep++) puts(*ep);
	exit(EXIT_SUCCESS);
}
