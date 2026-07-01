#define _GNU_SOURCE
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#include<sys/wait.h>
static void handler(int sig) {
	printf("Receive signal %d (%s)\n",sig,strsignal(sig));
}
int main(int argc,char *argv[]) {
	sigset_t blockMask,origMask;
	struct sigaction sa,origSa;
	sigemptyset(&blockMask);
	sigemptyset(&sa.sa_mask);
	sigaddset(&blockMask,SIGCHLD);
	int status;
	sa.sa_flags=0;
	sa.sa_handler=handler;
	if (sigprocmask(SIG_BLOCK,&blockMask,&origMask)==-1) errExit("sigprocmask");
	if (sigaction(SIGCHLD,&sa,&origSa)==-1) errExit("sigaction");
	int childPid;
	switch((childPid=fork())) {
		case -1:errExit("fork");
		case 0:
			_exit(EXIT_SUCCESS);
		default:
            sleep(1);
			if(waitpid(childPid,&status,0)==-1) errExit("waitpid");


	}
	if (sigprocmask(SIG_SETMASK,&origMask,NULL)==-1) errExit("sigprocmask");
	if (sigaction(SIGCHLD,&origSa,NULL)==-1) errExit("sigaction");
	exit(EXIT_SUCCESS);
}
