#define _GNU_SOURCE //Get strsignal() declaration from <string.h>
#include<signal.h>
#include<time.h>
#include "../Lib/signal_functions.h" /*Declaration of printSigMask()
and printPendingSigs()*/
#include "../Lib/tlpi_hdr.h"
#include<stdio.h>
#include<errno.h>
static volatile sig_atomic_t gotSigquit=0;
static void handler(int sig) {
	printf("Caught signal %d (%s)\n",sig,strsignal(sig));
	if (sig==SIGQUIT) gotSigquit=1;
}
int main(int argc,char *argv[]) {
	int loopNum;
	time_t startTime;
	sigset_t origMask,blockMask;
	struct sigaction sa;
	printSigMask(stdout,"Initial signal mask is: \n");
	sigemptyset(&sa.sa_mask);
	sigemptyset(&blockMask);
	sigaddset(&blockMask,SIGINT);
	sigaddset(&blockMask,SIGQUIT);
	if (sigprocmask(SIG_BLOCK,&blockMask,&origMask)==-1) errExit("sigprocmask - SIG_BLOCK");
	sa.sa_flags=0;
	sa.sa_handler=handler;
	if (sigaction(SIGINT,&sa,NULL)==-1) errExit("sigaction");
	if (sigaction(SIGQUIT,&sa,NULL)==-1) errExit("sigaction");
	for (loopNum=1;!gotSigquit;loopNum++) {
		printf("=== LOOP %d\n",loopNum);
		//Simulate a critical section by delaying a few seconds
		printSigMask(stdout,"Stating critical section, signal mask is: \n");
		for (startTime=time(NULL);time(NULL)<startTime+4;) continue;//Run for few second elapsted time
		printPendingSigs(stdout,"Befor sigsuspend() - pending signals:\n");
		if (sigpending(&origMask)==-1&&errno!=EINTR) errExit("sigsuspend");
	}
	if (sigprocmask(SIG_SETMASK,&origMask,NULL)==-1) errExit("sigprocmask - SIG_SETMASK");
	printSigMask(stdout,"==Exited loop\nRestored signal mask to:\n");
	//Do other processing
	exit(EXIT_SUCCESS);
}
