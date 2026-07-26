#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include "../Lib/tlpi_hdr.h"
static void handler(int sig) {
	printf("PID=%ld receive signal %d (%s)\n",(long)getpid(),sig,strsignal(sig));
}
int main(int argc,char *argv[]) {

	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	sa.sa_handler=handler;
	if (sigaction(SIGHUP,&sa,NULL)==-1) errExit("sigaction");
	switch(fork()) {
		case -1: errExit("fork");
	}
	printf("PID=%ld, PPID=%ld, GRID=%ld, SID=%ld\n",
	(long)getpid(),(long)getppid(),(long)getpgrp(),(long)getsid(0));
	pause();

	exit(EXIT_SUCCESS);
}
