#define _POSIX_C_SOURCE 199309
#define _GNU_SOURCE
#include<signal.h>
#include<time.h>
#include<sys/time.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
static void handler(int sig, siginfo_t *si,void *utc) {
	printf("Receive signal %d (%s)\n",sig,strsignal(sig));
	timer_t* tid=si->si_value.sival_ptr;
	printf("TimerID: %ld\n",(long)*tid);
	printf("Overun: %d\n",timer_getoverrun(*tid));
}
int main(int argc,char *argv[]) {
	if (argc<3||strcmp(argv[1],"--help")==0) usageErr("%s second frequent\n");
	int second=getInt(argv[1],0,"seconds");
	int fre=getInt(argv[2],0,"frequent");
	timer_t tid;
	struct sigevent evt;
    evt.sigev_notify=SIGEV_SIGNAL;
    evt.sigev_signo=SIGRTMAX;
    evt.sigev_value.sival_ptr=&tid;
	if (timer_create(CLOCK_REALTIME,&evt,&tid)==-1) errExit("timer_create");
	 sigset_t block;
	sigemptyset(&block);
	sigaddset(&block,SIGRTMAX);
	if (sigprocmask(SIG_BLOCK,&block,NULL)==-1) errExit("sigprocmask");
	struct itimerspec it;
	it.it_interval.tv_sec=fre;
	it.it_interval.tv_nsec=0;
	it.it_value.tv_sec=second;
	it.it_value.tv_nsec=0;
	if (timer_settime(tid,0,&it,NULL)==-1) errExit("timer_settime");
//	struct sigaction sa;
//	sa.sa_flags=SA_SIGINFO;
//	sigemptyset(&sa.sa_flags);
//	sa.sa_sigaction=handler;
//	if (sigaction(SIGRTMAX,&sa,NULL)==-1) errExit("sigaction");
	 siginfo_t si;
	for (;;) {
        int sig=sigwaitinfo(&block,&si);
        if (sig==-1) errExit("sigwaitinfo");
        printf("Receive signal %d (%s)\n",sig,strsignal(sig));
        timer_t* tid=si.si_value.sival_ptr;
        printf("TimerID: %ld\n",(long)*tid);
        printf("Overun: %d\n",timer_getoverrun(*tid));


	}
}
