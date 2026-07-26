
#define _GNU_SOURCE
#include<signal.h>
#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include "../Lib/tlpi_hdr.h"
static void *func(void *arg) {
	int t=*((int *)arg);
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,t);
	int s=pthread_sigmask(SIG_BLOCK,&mask,NULL);
	if (s!=0) errExitEN(s,"pthread_sigmask");
	int sig;
	s=sigwait(&mask,&sig);
	if (s!=0) errExitEN(s,"sigwait");
	printf("receive signal %d (%s) at %lu\n",sig,strsignal(sig),(long)pthread_self());
	return NULL;

}

int main(int argc,char *argv[]) {
	pthread_t t1,t2;
	int s;
	int sig1=SIGUSR1,sig2=SIGUSR2;
	s=pthread_create(&t1,NULL,func,&sig1);
	if (s!=0) errExitEN(s,"pthread_create");
	s=pthread_create(&t2,NULL,func,&sig2);
	if (s!=0) errExitEN(s,"pthread_create");
	sleep(2);
	s=pthread_kill(t1,sig1);
	if (s!=0) errExitEN(s,"pthread_kill");
	s=pthread_kill(t2,sig2);
	if (s!=0) errExitEN(s,"pthread_kill");
	s=pthread_join(t1,NULL);
	if (s!=0 ) errExitEN(s,"pthread_join");
	s=pthread_join(t2,NULL);
	if (s!=0) errExitEN(s,"pthread_join");
	exit(EXIT_SUCCESS);
}
