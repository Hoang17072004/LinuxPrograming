#include<sys/wait.h>
#include<stdio.h>
#include "../Lib/print_wait_status.h" //Declares printWaitStatus()
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	int status ;
	pid_t childPid;
	if (argc>1&&strcmp(argv[1],"--help")==0)
		usageErr("%s [exit-status]\n",argv[0]);
	switch(fork()) {
		case -1: errExit("fork");
		case 0:
			/*Child:either exits imediately with given status
			or loops waiting for signals*/
			printf("Child started with PID = %ld\n",(long)getpid());
			if (argc>1) {
				exit(getInt(argv[1],0,"exit_status"));
			} else {
				for (;;) pause();
			}
			exit(EXIT_FAILURE);//Not reached but good practive
		default:
			/*Parent: repeated wait on child until it
			either exits or is terminated by a signal*/
			for (;; ) {
				childPid=waitpid(-1,&status,WUNTRACED
#ifdef WCONTINUED
						|WCONTINUED
#endif
				);
				if (childPid==-1) errExit("waitpid");
				//Print status in hex,and as separate decimal bytes
				printf("waitpid() return PID=%ld; status =0x%04x (%d,%d	)\n",
						(long)childPid,
						(unsigned int)status,status>>8,status &0xff);
				printWaitStatus(NULL,status);
				if (WIFEXITED(status)||WIFSIGNALED(status)) exit(EXIT_SUCCESS);
			}
	}
}
