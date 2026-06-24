#define _POSIX_C_SOURCE 199309
#define _GNU_SOURCE
#include<signal.h>
#include<time.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#include<string.h>
static void handler(int sig,siginfo_t *si,void *uc) {
    printf("Receive SIGNAL %d(%s)\n",sig,strsignal(sig));
    timer_t* tid=si->si_value.sival_ptr;
    printf("TimerID: %ld\n",(long)*tid);
    printf("Overun: %d\n",timer_getoverrun(*tid));
}
int main(int argc,char *argv[]) {
    if (argc<3||strcmp(argv[1],"--help")==0) usageErr("%s seconds frequent\n",argv[0]);
    int seconds=getInt(argv[1],0,"seconds");
    int fre=getInt(argv[2],0,"frequent");
    struct sigevent ev;
    ev.sigev_notify=SIGEV_SIGNAL;
    ev.sigev_signo=SIGRTMAX;
    timer_t id;
    ev.sigev_value.sival_ptr=&id;
    struct sigaction sa;
    sa.sa_flags=SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction=handler;

    if (timer_create(CLOCK_REALTIME,&ev,&id)==-1) errExit("timer_create");
    struct itimerspec it;
    it.it_interval.tv_sec=fre;
    it.it_interval.tv_nsec=0;
    it.it_value.tv_sec=seconds;
    it.it_value.tv_nsec=0;

    if (sigaction(SIGRTMAX,&sa,NULL)==-1) errExit("sigaction");
    if (timer_settime(id,0,&it,NULL)==-1) errExit("timer_settime");

    for (;;) {
        pause();
    }
}
