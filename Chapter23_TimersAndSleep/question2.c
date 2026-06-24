
#define _XOPEN_SOURCE 600
#define _GNU_SOURCE
#include<time.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#include<signal.h>
#include<sys/time.h>
#include<string.h>
#include<errno.h>
static void handler(int sig) {
    printf("Receive SIGNAL : %d (%s)\n",sig,strsignal(sig));
}
int main(int argc,char *argv[]) {
	if (argc<2||strcmp(argv[1],"--help")==0)
		usageErr("%s seconds\n",argv[0]);
	int seconds=getInt(argv[1],0,"seconds");
	struct timespec remain,request;
	request.tv_sec=seconds;
	request.tv_nsec=0;
	struct sigaction sa;
	sa.sa_flags=0;
	sa.sa_handler=handler;
    struct timeval start,cur;
    if (clock_gettime(CLOCK_REALTIME,&request)==-1) errExit("clock_gettime");
    request.tv_sec+=seconds;
    if (sigaction(SIGINT,&sa,NULL)==-1) errExit("sigaction");
	for (;;) {

		int s=clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&request,&remain);
		if (s!=0&&s!=EINTR) errExit("clock_nanosleep");
		if (s==0) break;
		if (gettimeofday(&cur,NULL)==-1) errExit("gettimeofday");
		printf("Sleep for %9.6f secs\n",(cur.tv_sec-start.tv_sec)+(cur.tv_usec-start.tv_usec)/1000000.0);
        printf("Remain Time: %2ld.%09ld secs\n",(long)remain.tv_sec,(long)remain.tv_nsec);

	}
}
