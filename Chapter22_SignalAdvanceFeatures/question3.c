
#include<signal.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	if (argc<2||strcmp(argv[1],"--help")==0)
		usageErr("%s numbers\n",argv[0]);
	long num=getLong(argv[1],GN_GT_0,"num");

	sigset_t recMask,blockMask;
	sigemptyset(&recMask);
	sigaddset(&recMask,SIGUSR1);
	sigfillset(&blockMask);
	if (sigprocmask(SIG_SETMASK,&blockMask,NULL)==-1) errExit("sigprocmask");
	pid_t childpid=fork();

	if (childpid==-1) errExit("fork");

	if (childpid==0) {
		//Child
		int sig;
		siginfo_t si;
		for (int i=0;i<num;i++) {
			if (kill(getppid(),SIGUSR1)==-1) errExit("kill");
			if ((sig=sigwaitinfo(&recMask,&si))==-1) errExit("sigwait");
			printf("At child: got signal :%d (%s)\n",sig,strsignal(sig));

		}
		exit(EXIT_SUCCESS);
	} else {
		int sig;
		siginfo_t si;
		for (int i=0;i<num;i++) {

			if ((sig=sigwaitinfo(&recMask,&si))==-1) errExit("sigwaitinfo");
			if (kill(childpid,SIGUSR1)==-1) errExit("kill");
			printf("At parent: got signal: %d (%s)\n",sig,strsignal(sig));

		}
		exit(EXIT_SUCCESS);
	}
}
