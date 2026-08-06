#include<sys/resource.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	if (argc<2||strcmp(argv[1],"--help")==0)
		usageErr("%s numprocess\n",argv[0]);
	int num=getInt(argv[1],0,"num");
	for (int i=0;i<num;i++) {
		switch(fork()) {
			case -1:errExit("fork");
			case 0:
                pause();
				_exit(EXIT_SUCCESS);
			default:
				break;

		}
	}
	//Chỉ parent
	if (getpid()==getpgrp()) {
		printf("Parent \n");
	}
	struct rlimit rlim;
	rlim.rlim_cur=num-1;
	rlim.rlim_max=num+1;
	if (setrlimit(RLIMIT_NPROC,&rlim)==-1) errExit("setrlimit");
	printf("No affect");
	switch(fork()) {
         case -1: errExit("fork");
         case 0: _exit(EXIT_SUCCESS);
         default: break;
	}
	printf("Add more no affect\n");
	exit(EXIT_SUCCESS);
}
