#include<signal.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
static void handler(int sig) {
	printf("Receive signal %d %s\n",sig,strsignal(sig));
}
int main(int argc,char *argv[]) {
	if (argc<2||strcmp(argv[1],"--help")==0)
		usageErr("%s time\n",argv[0]);
	sigset_t blockMask,prevMask;
	sigfillset(&blockMask);
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=SA_RESTART;
	sa.sa_handler=handler;
	for (int sig=1;sig<=SIGRTMAX;sig++) {
		if (sig==SIGKILL||sig==SIGSTOP) continue;
		if (sigaction(sig,&sa,NULL)==-1) perror("sigaction");
	}
	printf("PID: %ld\n",getpid());
	int t=getInt(argv[1],GN_GT_0,"time");
	if (sigprocmask(SIG_SETMASK,&blockMask,&prevMask)==-1) errExit("sigprocmask");
	sleep(t);
	if (sigprocmask(SIG_SETMASK,&prevMask,NULL)==-1) errExit("sigprocmask");
	for (;;) pause();
	exit(EXIT_SUCCESS);
}
