
#include<sys/resource.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	if (argc<2||strcmp(argv[1],"--help")==0)
		usageErr("%s command arg...\n");
	pid_t childPid;
	switch((childPid=fork()) ) {
		case -1:errExit("fork");
		case 0:
			printf("Child PID %ld\n",(long)getpid());
			execvp(argv[1],&argv[1]);
			_exit(EXIT_SUCCESS);
		default:
			if (waitpid(childPid,NULL,0)==-1) errExit("waitpid");
	}
	struct rusage resusage;
	if (getrusage(RUSAGE_CHILDREN,&resusage)==-1) errExit("getrusage\n");
	printf("Usage time: %ld.%06ld\n",resusage.ru_utime.tv_sec,resusage.ru_utime.tv_usec);
	printf("User CPU time:   %ld.%06ld s\n",
       (long) resusage.ru_utime.tv_sec, (long) resusage.ru_utime.tv_usec);
printf("System CPU time: %ld.%06ld s\n",
       (long) resusage.ru_stime.tv_sec, (long) resusage.ru_stime.tv_usec);
	exit(EXIT_SUCCESS);
}
