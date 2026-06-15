#define _GNU_SOURCE
#include<signal.h>
#include<stdio.h>
#include "../Lib/signal_functions.h"
#include "../Lib/tlpi_hdr.h"
static int sigCnt[NSIG];
static volatile sig_atomic_t gotSigint=0;
static void handler(int sig) {
	if (sig!=SIGINT) sigCnt[sig]++;
	else gotSigint=1;
}
int main(int argc,char *argv[]) {
	int n,numSecs;
	sigset_t pendingMask,blockingMask,emptyMask;
	printf("%s: PID is %ld\n",argv[0],(long)getpid());
	struct sigaction act;
	memset(&act,0,sizeof(act));
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	act.sa_handler=handler;
	for (n = 1; n < NSIG; n++) {
        if (n == SIGKILL || n == SIGSTOP)
            continue;

        if (sigaction(n, &act, NULL) == -1) {
            // Không thoát, chỉ in cảnh báo
            fprintf(stderr, "Can't catch signal %d\n", n);
        }
    }

	if (argc>1) {
		numSecs=getInt(argv[1],GN_GT_0,NULL);
		sigfillset(&blockingMask);
		if (sigprocmask(SIG_SETMASK,&blockingMask,NULL)==-1) errExit("sigprocmask");
		printf("%s: sleeping for %d seconds\n",argv[0],numSecs);
		sleep(numSecs);
		if(sigpending(&pendingMask)==-1) errExit("sigpending");
		printf("%s: pending signals are: \n",argv[0]);
		printSigset(stdout,"\t\t",&pendingMask);
		sigemptyset(&emptyMask);
		if (sigprocmask(SIG_SETMASK,&emptyMask,NULL)==-1) errExit("sigprocmask");
	}
	while(!gotSigint) continue;
	for (n=1;n<NSIG;n++)
		if (sigCnt[n]!=0)
			printf("%s: signal %d caught %d times%s\n",argv[0],n,sigCnt[n],(sigCnt[n]==1)?"":"s");
	exit(EXIT_SUCCESS);
}
