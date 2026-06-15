
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include "../Lib/tlpi_hdr.h"
static void handler1(int sig){
	printf("caught sigusr 1\n");
}
static void handler2(int sig) {
	printf("caught sigusr 2\n");
}
int main(int argc,char *argv[]) {
	if (argc<2) usageErr("%s [1,2]\n",argv[0]);
	int type=getInt(argv[1],GN_GT_0,"type");
	struct sigaction sa;
	memset(&sa,0,sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler=(type==1)?handler1:handler2;
	sa.sa_flags=(type==1)?SA_RESETHAND:SA_NODEFER;
	if (sigaction(SIGUSR1,&sa,NULL)==-1) errExit("sigaction");
	long pid=(long)getpid();
	if (kill(pid,SIGUSR1)==-1) errExit("kill");
	if (raise(SIGUSR1)) errExit("kill");

}
