#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	printf("initial value of USER: %s\n",getenv("USER"));
	if (putenv("USER=britta")!=0) errExit("putenv");
	execl("/usr/bin/printenv","printenv","USER","SHELL",(char*)NULL);
	errExit("execl");//If we get here,something went wrong
}
