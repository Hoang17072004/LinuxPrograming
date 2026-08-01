#include<sched.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#include<sys/wait.h>
int main(int argc,char *argv[]) {
	if (argc<4||strcmp(argv[1],"--help")==0)
		usageErr("%s policy priority command arg...\n",argv[0]);
	if (strchr("rf",argv[1][0])==NULL) usageErr("policy must in rf\n");
	int pol,prio;
	pol=(argv[1][0]=='r')?SCHED_RR:SCHED_FIFO;
	prio=getInt(argv[2],0,"priority");
	struct sched_param sp;
	sp.sched_priority=prio;
	pid_t childPid;
	switch((childPid=fork())) {
		case -1: errExit("fork");
		case 0:
			printf("Process PID: %ld\n",(long)getpid());
			if (sched_setscheduler(0,pol,&sp)==-1) errExit("sched_setscheduler");
			execvp(argv[3],&argv[3]);
		default:
			break;
	}
	if (wait(NULL)==-1) errExit("wait");
	exit(EXIT_SUCCESS);

}
