#include<sys/resource.h>
#include "print_rlimit.h"
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	struct rlimit rl;
	setbuf(stdout,NULL);
	int j;
	pid_t childPid;
	if (argc<2||argc>3||strcmp(argv[1],"--help")==0)
		usageErr("%s soft-limt [hard-limit]\n",argv[0]);
	printRlimit("Initial maximum process limits: ",RLIMIT_NPROC);
	//Set new process limits (hard==soft if not specified)
	rl.rlim_cur=(argv[1][0]=='i')?RLIM_INFINITY:getInt(argv[1],0,"soft-liit");
	rl.rlim_max=(argc==2)?rl.rlim_cur:(argv[2][0]=='i')?RLIM_INFINITY:getInt(argv[2],0,"hard-limit");
	if(setrlimit(RLIMIT_NPROC,&rl)==-1) errExit("setrlimit");
	printRlimit("New maximum process limit: ",RLIMIT_NPROC);
	//Create as many children as posible
	for (j=1;;j++) {
		switch(childPid=fork()) {
			case -1: errExit("fork");
			case 0:_exit(EXIT_SUCCESS);//child
			default: //Parent : display message about each new child and let the resulting zombie accumulate
				printf("Child %d (PID=%ld) started\n",j,(long)childPid);
				break;
		}
	}
}
