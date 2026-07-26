#include<signal.h>
#include "../Lib/tlpi_hdr.h"
#include<stdio.h>
#include<errno.h>
static void tstpHandler(int sig) {//Handler for SIGTSTP
	sigset_t tspMask,prevMask;
	int savedErrno;
	struct sigaction sa;
	savedErrno=errno;//In case we change 'errno' here
	printf("Caught SIGTSTP\n");
	if (signal(SIGTSTP,SIG_DFL)==SIG_ERR) errExit("signal");
	raise(SIGTSTP);
	//Unblock SIGTSTP; the pending SIGTSTP immediately suspends the program
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask,SIGTSTP);
	if (sigprocmask(SIG_UNBLOCK,&tspMask,&prevMask)==-1) errExit("sigprocmask");
	//Execution resumes here after SIGCONT
	if (sigprocmask(SIG_SETMASK,&prevMask,NULL)==-1) errExit("sigprocmask");
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=SA_RESTART;
	sa.sa_handler=tstpHandler;
	if (sigaction(SIGTSTP,&sa,NULL)==-1) errExit("sigaction");
	printf("Exiting SIGTSTP handler\n");
	errno=savedErrno;
}
int main(int argc,char *argv[]) {
	struct sigaction sa;
	//Only establish handler for SIGTSTP if it is not being ignored
	if (sigaction(SIGTSTP,NULL,&sa)==-1) errExit("sigaction");
	if (sa.sa_handler!=SIG_IGN) {
		sigemptyset(&sa.sa_mask);
		sa.sa_flags=SA_RESTART;
		sa.sa_handler=tstpHandler;
		if (sigaction(SIGTSTP,&sa,NULL)==-1) errExit("sigaction");
	}
	for (;;) {
		//Wait for signals
		pause();
		printf("Main\n");
	}
}
