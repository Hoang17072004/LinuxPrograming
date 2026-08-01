#define _GNU_SOURCE
#include<sched.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/times.h>
#include<time.h>
#include<sys/wait.h>
#include<string.h>
#include "../Lib/tlpi_hdr.h"
#define BUF_SIZE 4096
#define N_ITER 100

void setcpu(int cpu) {
	cpu_set_t s;
	CPU_ZERO(&s);
	CPU_SET(cpu,&s);
	if (sched_setaffinity(getpid(),sizeof(cpu_set_t),&s)==-1) errExit("sched_setaffnitiy");

}
int main(int argc,char *argv[]) {
	int fi=0,se=0;
	struct timespec start,end;
	if (clock_gettime(CLOCK_MONOTONIC,&start)==-1) errExit("clock_gettime");
	if (argc<2) usageErr("%s cpu0 [cpu1]\n",argv[0]);
	int fd[2];
	if (pipe(fd)==-1) errExit("pipe");
	pid_t childPid;
	fi=getInt(argv[1],0,"first");
	se=fi;
	if (argc>2) se=getInt(argv[2],0,"second");
	//times
	char buf[BUF_SIZE];
	switch((childPid=fork())) {
		case -1: errExit("fork");
		case 0:
			setcpu(fi);
			close(fd[1]);

			for (int i=0;i<N_ITER;i++) {
				if (read(fd[0],buf,BUF_SIZE)<=0) errExit("read");
			}
			close(fd[0]);
			_exit(EXIT_SUCCESS);
		default:
			setcpu(se);
			close(fd[0]);
//			char buf[BUF_SIZE];
			for (int i=0;i<N_ITER;i++) {
				if (write(fd[1],buf,BUF_SIZE)<=0) errExit("write");
			}
			close(fd[1]);
			wait(NULL);
			if (clock_gettime(CLOCK_MONOTONIC,&end)==-1) errExit("clock_gettime");
			double elapsted=(end.tv_sec-start.tv_sec)+(end.tv_nsec-start.tv_nsec)/1e9;
			printf("Elasped time: %.3f seconds (%s CPU)\n",elapsted,(argc>2)?"different":"same");


	}
	exit(EXIT_SUCCESS);
}
