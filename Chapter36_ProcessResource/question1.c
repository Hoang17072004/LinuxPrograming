#include<stdio.h>
#include<sys/resource.h>
#include "../Lib/tlpi_hdr.h"
#include<sys/wait.h>
int main(int argc,char *argv[]) {
	if (argc<2||strcmp(argv[1],"--help")==0)
		usageErr("%s numchild\n",argv[0]);
	int num=getInt(argv[1],0,"num");
	for (int i=0;i<num;i++) {
		switch(fork()) {
			case -1: errExit("fork");
			case 0:
				printf("Child %lld terminated\n",(long)getpid());
				_exit(EXIT_SUCCESS);
		}
	}
	printf("Parent before wait\n");
	struct rusage resusage;
	if (getrusage(RUSAGE_CHILDREN,&resusage)==-1) errExit("getrusage");
	printf("User CPU time: %ld.%06ld\n",resusage.ru_utime.tv_sec,resusage.ru_utime.tv_usec);
	wait(NULL);
	printf("Wait success on parent\n");
	if (getrusage(RUSAGE_CHILDREN,&resusage)==-1) errExit("getrusage");
	printf("User CPU time: %ld.%06ld\n",resusage.ru_utime.tv_sec,resusage.ru_utime.tv_usec);
	exit(EXIT_SUCCESS);
}
