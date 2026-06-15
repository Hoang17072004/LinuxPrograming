#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
static void handler(int sig) {
	printf("Received %d\n",sig);
}
int main(int argc,char *argv[]) {
	struct sigaction sa;
	memset(&sa,0,sizeof(sa));
	sa.sa_handler=handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	sigset_t blockSet;
	sigemptyset(&blockSet);
	sigaddset(&blockSet,SIGUSR1);
	if (sigprocmask(SIG_SETMASK,&blockSet,NULL)==-1) errExit("sigprocmask");
	if(sigaction(SIGUSR1,&sa,NULL)==-1) errExit("sicaction");
	raise(SIGUSR1);
	sa.sa_handler=SIG_IGN;
	if(sigaction(SIGUSR1,&sa,NULL)==-1) errExit("sigaction");
	raise(SIGUSR1);
	if (sigprocmask(SIG_UNBLOCK,&blockSet,NULL)==-1) errExit("sigprocmask");
	raise(SIGUSR1);
	printf("Pending");
	pause();
	exit(EXIT_SUCCESS);
}
