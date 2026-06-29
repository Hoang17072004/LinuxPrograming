
#define _GNU_SOURCE
#include<sys/wait.h>
#include<signal.h>
#include <stdio.h>
#include "../Lib/tlpi_hdr.h"
#include<string.h>
static void printStatus(siginfo_t *si) {
	if (si->si_code==CLD_EXITED) {
		printf("Exited , status: %d\n",si->si_status);
	} else if (si->si_code==CLD_KILLED) {
		printf("Kill signal %d (%s)\n",si->si_status,strsignal(si->si_status));
	} else if (si->si_code==CLD_STOPPED) {
		printf("Stop by signal %d (%s)\n",si->si_status,strsignal(si->si_status));

	}else if(si->si_code==CLD_CONTINUED) {
		printf("Child continue by signal %d (%s)\n",si->si_status,strsignal(si->si_status));
	}else {
		printf("Don't know\n");
	}
	printf("PID: %ld\n",(long)si->si_pid);
	printf("UID: %ld\n",(long)si->si_uid);
}
int main(int argc,char *argv[]) {
	if (argc>1&&strcmp(argv[1],"--help")==0)
		usageErr("%s seconds\n");
	long childPid;
	siginfo_t info;
	memset(&info,0,sizeof(siginfo_t));
	switch((fork()) ) {
		case -1: errExit("fork");
		case 0:
            printf("Child PID: %ld\n",(long)getpid());
			if (argc>1) _exit(getInt(argv[1],0,"seconds"));
			else for (;;) pause();
			exit(EXIT_FAILURE);
		default:

			for (;;) {
				childPid=waitid(P_ALL,0,&info,WEXITED|WSTOPPED|WCONTINUED);
				if (childPid==-1) errExit("waitid\n");
//				printf("Return PID: %ld\n",(long)info.si_pid);
				printStatus(&info);
				if (info.si_code==CLD_EXITED||info.si_code==CLD_KILLED) exit(EXIT_SUCCESS);
			}
	}
}
