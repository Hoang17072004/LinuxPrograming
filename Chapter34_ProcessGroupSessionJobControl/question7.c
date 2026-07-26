
#define _GNU_SOURCE
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
static void handler(int sig) {
	printf("PID=%ld receive signal %d (%s)\n",(long)getpid(),sig,strsignal(sig));
}
int main(int argc,char *argv[]) {
	pid_t childPid;
	struct sigaction sa;
	sa.sa_flags=0;
	sigemptyset(&sa.sa_mask);
	printf("Parent PID=%ld, GRID=%ld\n",(long)getpid(),(long)getpgrp());
	sa.sa_handler=(argc>1)?handler:SIG_DFL;
	if (sigaction(SIGTTIN,&sa,NULL)==-1)errExit("sigaction");
	if (sigaction(SIGTTOU,&sa,NULL)==-1) errExit("sigaction");
	if (sigaction(SIGTSTP,&sa,NULL)==-1) errExit("sigaction");
	switch((childPid=fork())) {
		case -1: errExit("fork");
		case 0:
            printf("Child PID=%ld , PPID=%ld, GRID=%ld\n",(long)getpid(),(long)getppid(),(long)getpgrp());
			sleep(4);
			pause();
			printf("Child receive\n");
			_exit(EXIT_SUCCESS);
		default:
			break;
	}
	exit(EXIT_SUCCESS);
}
