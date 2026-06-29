
#define _GNU_SOURCE
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#define CMD_SIZE 200
//static volatile Boolean isReceived=0;
//static void handler(int sig) {
//	printf("Receive signal %d (%s)\n",sig,strsignal(sig));
////	isReceived=1;
//}
int main(int argc,char *argv[]) {
	char cmd[CMD_SIZE];
	pid_t childPid;
	setbuf(stdout,NULL);
	sigset_t blockMask,emptyMask;
//	struct sigaction sa;
//	sigemptyset(&sa.sa_mask);
//	sa.sa_flags=0;
//	sa.sa_handler=handler;
//	if (sigaction(SIGCHLD,&sa,NULL)==-1) errExit("sigaction");
	sigemptyset(&blockMask);
	sigemptyset(&emptyMask);
	sigaddset(&blockMask,SIGCHLD);
	if(sigprocmask(SIG_BLOCK,&blockMask,NULL)==-1) errExit("sigprocmask");
	snprintf(cmd,CMD_SIZE,"ps aux| grep %s",argv[0]);
	cmd[CMD_SIZE-1]='\0';
//	long childPid=0;
	siginfo_t info;
	switch((childPid=fork())) {
		case -1: errExit("fork");
		case 0:
			printf("Child PID: %ld\n",(long)getpid());
			_exit(EXIT_SUCCESS);
		default:
			int si;
			si=sigwaitinfo(&blockMask,&info);
			if (si==-1) errExit("status");
			printf("Receive signal : %d (%s)\n",si,strsignal(si));
			printf("From PID: %ld\n",(long)info.si_pid);
			sleep(3);
			system(cmd);
			sleep(3);
			if (kill(childPid,SIGKILL)==-1) errExit("sigkill");
			sleep(3);
			system(cmd);
			exit(EXIT_SUCCESS);
	}
}
