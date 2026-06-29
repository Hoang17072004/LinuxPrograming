#include<sys/wait.h>
#include<time.h>
#include<stdio.h>
#include<errno.h>
#include "../Lib/curr_time.h"
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	int numDead;//Number of children so far waited for
	pid_t childPid;//PID of waited child
	int j;
	if (argc<2||strcmp(argv[1],"--help")==0) usageErr("%s sleep-time ...\n",argv[0]);
	setbuf(stdout,NULL);//Disable buffering of stdout
	for (j=1;j<argc;j++) {
		//Create one child for each argument
		switch(fork()) {
			case -1: errExit("fork");
			case 0:
				printf("[%s] child %d started with PID %ld, sleeping %s seconds\n",
						currTime("%T"),j,(long)getpid(),argv[j]);
                sleep(getInt(argv[j],0,"seconds"));
				_exit(EXIT_SUCCESS);
			default:break;
		}
	}
	numDead=0;
	for (;;) {
		childPid=wait(NULL);//Parent wait for each child to exit
		if (childPid==-1) {
			if (errno==ECHILD) {
				printf("No more children - bye!\n");

				exit(EXIT_SUCCESS);
			} else {
				errExit("wait");
			}
		}
		numDead++;
		printf("[%s] wait() returned child PID %ld (numDead=%d)\n",currTime("%T"),(long)childPid,numDead);
	}
}
