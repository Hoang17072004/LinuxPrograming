#include<signal.h>
#include<libgen.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#define CMD_SIZE 200
int main(int argc,char *argv[]) {
	char cmd[CMD_SIZE];
	pid_t childPid;
	setbuf(stdout,NULL);//Disable buffering of stdout
	printf("Parent PID=%ld\n",(long)getpid());
	switch(childPid=fork()){
		case -1: errExit("fork");
		case 0://Child: imediately exist to become zombie
		printf("Child (PID=%ld) existing\n",(long)getpid());
		_exit(EXIT_SUCCESS);
		default://Parent
			sleep(3);//Give child a change to start and exit
			snprintf(cmd,CMD_SIZE,"ps | grep %s",basename(argv[0]));
			cmd[CMD_SIZE-1]='\0'; //Ensure string is null-terminated
			system(cmd);//View zombie child
			//Now send the "sure kill" signal to zoombie
			if (kill(childPid,SIGKILL)==-1) errMsg("kill");
			sleep(3);//Give child a change to react to signal
			printf("After sending SIGKILL to zoobie (PID=%ld):\n",(long)childPid);
			system(cmd);
			exit(EXIT_SUCCESS);
	}
}
