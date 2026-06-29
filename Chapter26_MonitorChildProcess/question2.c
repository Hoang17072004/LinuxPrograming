#include<signal.h>
#include<sys/wait.h>
#include "../Lib/curr_time.h"
#include"../Lib/tlpi_hdr.h"
#include<stdio.h>
int main(int argc,char *argv[]) {
	switch(fork()) {
		case -1: errExit("fork");
		case 0: //parent
			switch(fork()) {
				case -1: errExit("fork");
				case 0://child
					sleep(5);
					printf("Child PID: %ld,Parent PID: %ld\n",(long)getpid(),(long)getppid());
					exit(EXIT_SUCCESS);
				default: //parent
					exit(EXIT_SUCCESS);
			}
		default: //grandparent
			sleep(10);
			exit(EXIT_SUCCESS);
	}
}
