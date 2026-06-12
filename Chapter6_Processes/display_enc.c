#include "../Lib/tlpi_hdr.h"
extern char **environ;
#include<stdio.h>
int main(int argc,char *argv[]) {
	char **ep;
	for (ep=environ;ep!=NULL;ep++) puts(*ep);
	exit(EXIT_SUCCESS);
}
