#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	pid_t childPid;
	switch((childPid=fork())) {
		case -1: errExit("fork");
		case 0:
			sleep(2);
			char *arg[]={"/bin/sleep","10",NULL};
			execve(arg[0],arg,NULL);
			errExit("execve");
			exit(EXIT_FAILURE);
		default:
			if (setpgid(childPid,childPid)==-1) errExit("setgid");
            printf("CHange before success\n");
			sleep(4);
			if (setpgid(childPid,childPid+1)==-1) errExit("setpgid");
            printf("change after error\n");
			exit(EXIT_SUCCESS);

	}
	exit(EXIT_SUCCESS);

}
