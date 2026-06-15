
#define _GNU_SOURCE
#include<signal.h>
#include "../Lib/signal_functions.h"//Declaration ò printSigset()
#include "../Lib/tlpi_hdr.h"
#include<stdio.h>
static int sigCnt[NSIG];//counts deliveries of each signal
static volatile sig_atomic_t gotSigint=0;//Set nonzero if SIGINT is delived
static void handler(int sig) {
	if (sig==SIGINT) gotSigint=1;
	else sigCnt[sig]++;
}
int main(int argc,char *argv[]) {
	int n,numSecs;
	sigset_t pendingMask,blockingMask,emptyMask;
	printf("%s: PID is %ld\n",argv[0],(long)getpid());
	for (n=1;n<NSIG;n++) //Samhandler for all signals
		(void)signal(n,handler);
	/*If a sleep time was specified ,temporarily block all signals,
		sleep (while another process sends us signals), and then
		display the mask of pending signals and unblock all signals
	*/
	if (argc>1) {
		numSecs=getInt(argv[1],GN_GT_0,NULL);
		sigfillset(&blockingMask);
		if (sigprocmask(SIG_SETMASK,&blockingMask,NULL)==-1) errExit("sigprocmask");
		printf("%s: sleeping for %d seconds\n",argv[0],numSecs);
		sleep(numSecs);
		if (sigpending(&pendingMask)==-1) errExit("sigpending");
		printf("%s: pending signals are: \n",argv[0]);
		printSigset(stdout,"\t\t",&pendingMask);
		sigemptyset(&emptyMask);//Unblock all signals
		if (sigprocmask(SIG_SETMASK,&emptyMask,NULL)==-1) errExit("sigprocmask");
	}
	while(!gotSigint) //Loop until SIGINT caught
		continue;
	for (n=1;n<NSIG;n++) //Display number of signals received
		if (sigCnt[n]!=0)
			printf("%s: signal %d caught %d times%s\n",argv[0],n,sigCnt[n],(sigCnt[n]==1)?"":"s");
	exit(EXIT_SUCCESS);
}
