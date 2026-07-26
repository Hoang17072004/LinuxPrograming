
#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	printf("Parent PID=%ld\n",(long)getpid());
	switch(fork()) {
		case -1: errExit("fork");
		case 0:
			sleep(2);
			int a;
			if (read(STDIN_FILENO,&a,sizeof(a))<=0) errExit("read");
			_exit(EXIT_SUCCESS);
		default: break;
	}
	_exit(EXIT_SUCCESS);
}
