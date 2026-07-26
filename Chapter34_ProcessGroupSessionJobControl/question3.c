#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	pid_t childPid;
	bool isLeader=false;
	switch((childPid=fork())) {
		case -1: errExit("fork");
	}
	printf("PID=%ld, PPID=%ld, GPID=%ld,SID=%ld\n",
	(long)getpid(),(long)getppid(),(long)getpgrp(),(long)getsid(0));
	if (getpgrp()==getpid()) {
		isLeader=true;
		printf("Process group leader\n");
	} else printf("Process group member\n");
	if (setsid()==-1) errExit(isLeader?"setsid at leader":"setsid at member");
	printf("Change success at %s\n",isLeader?"leader":"member");

}
