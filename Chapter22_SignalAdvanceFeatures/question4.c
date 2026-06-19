#include<stdio.h>
#include<signal.h>
int sigset(int sig,void(*handler)(int)) {
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	sa.sa_handler=handler;
	return sigaction(sig,&sa,NULL);
}
int sighold(int sig) {
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,sig);
	return sigprocmask(SIG_BLOCK,&mask,NULL);
}
int sigrelse(int sig) {
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,sig);
	return sigprocmask(SIG_UNBLOCK,&mask,NULL);
}
int sigignore(int sig) {
	struct sigaction sa;
	sa.sa_handler=SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	return sigaction(sig,&sa,NULL);
}
int sigpause(int sig) {
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,sig);
	return sigsuspend(&mask);
}
