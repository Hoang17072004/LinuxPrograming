#include<signal.h>
#include<sys/wait.h>
#include "../Lib/tlpi_hdr.h"
#include<stdio.h>
int main(int argc,char *argv[]) {
	setbuf(stdout,NULL);
	switch(fork()) {
		case -1: errExit("fork");
		case 0:
			sleep(10);
			printf("Child process\n");
			printf("Parent pid: %ld\n",(long)getppid());
			printf("Cur pid: %ld\n",(long)getpid());
			exit(EXIT_SUCCESS);
		default:
			printf("parent terminate\n");
			exit(EXIT_SUCCESS);
	}
}
