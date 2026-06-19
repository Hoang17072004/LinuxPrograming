#include<signal.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
static void handler(int sig) {
	printf("Recieved signal %d\n",sig);
}
int main(int argc,char *argv[]) {
	if (argc<2||strcmp(argv[1],"--help")==0)
		usageErr("%s time\n",argv[0]);
	int t=getInt(argv[1],GN_GT_0,"time");
	sigset_t blockMask,prevMask;
	printf("PID: %ld\n",(long)getpid());
	sigemptyset(&blockMask);
	sigaddset(&blockMask,SIGCONT);
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=SA_RESTART;
	sa.sa_handler=handler;
	if (sigaction(SIGCONT,&sa,NULL)==-1) errExit("sigaction");
	if (sigprocmask(SIG_BLOCK,&blockMask,&prevMask)==-1)errExit("sigblockmask");
	sleep(t);

	if (sigprocmask(SIG_SETMASK,&prevMask,NULL)==-1) errExit("sigblockmask");
	pause();
	printf("Unblock signal\n");
	exit(EXIT_SUCCESS);

}
