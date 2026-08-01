#include<stdio.h>
#include<sys/time.h>
#include<sys/resource.h>
#include "../Lib/tlpi_hdr.h"
#include<sys/wait.h>
int main(int argc,char *argv[]) {
	if (argc<3||strcmp(argv[1],"--help")==0)
		usageErr("%s priority filename\n",argv[0]);
	int prio=getInt(argv[1],0,"prio");
	pid_t childPid;
	char *childargv[]={argv[2],NULL};
	switch((childPid=fork())) {
		case -1: errExit("fork");
		case 0:
            printf("Process ID: %ld\n",(long)getpid());
			if (setpriority(PRIO_PROCESS,0,prio)==-1) errExit("setpriority");
			execvp(argv[2],&argv[2]);
		default:
			break;
	}
	if (waitpid(childPid,NULL,0)==-1) errExit("waitpid");
	exit(EXIT_SUCCESS);
}
