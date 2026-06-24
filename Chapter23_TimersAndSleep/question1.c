#include<signal.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#include<sys/time.h>
unsigned int myalarm(unsigned int seconds) {
	struct itimerval it;
	if (getitimer(ITIMER_REAL,&it)==-1) return -1;
	struct itimerval nit;
	nit.it_interval.tv_sec=nit.it_interval.tv_usec=0;
	nit.it_value.tv_sec=seconds;
	nit.it_value.tv_usec=0;
	if (setitimer(ITIMER_REAL,&nit,NULL)==-1) return -1;
	return it.it_value.tv_sec;
}
static void handler(int sig) {
	printf("Receive signal\n");
}
int main(int argc,char *argv[]) {
	if (argc<2||strcmp(argv[1],"--help")==0) usageErr("%s seconds\n",argv[0]);
	int second=getInt(argv[1],0,"seconds");
	printf("Start alarm\n");
	struct sigaction sa;
	sa.sa_flags=0;
	sa.sa_handler=handler;
	sigemptyset(&sa.sa_mask);
	if(sigaction(SIGALRM,&sa,NULL)==-1) errExit("sigaction");
	if (myalarm(second)==-1) errExit("myalarm");
	pause();

}
