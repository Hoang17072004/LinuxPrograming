#include<time.h>
#include<sched.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#define MAX_CPU_TIME 3.0
#define interval 0.25
#include<time.h>
#include<sys/times.h>
#include<sys/wait.h>
void consumeCPU() {

	struct tms buf;
	clock_t start=times(&buf);
	clock_t now;
	double cur=0.0;
	int last=0;
	int quater=0;
	while(cur<MAX_CPU_TIME) {
		now=times(&buf);
		cur=(double)(now-start)/sysconf(_SC_CLK_TCK);
		if(cur>quater*interval){
			printf("PID: %ld, eslapted: %.3f\n",getpid(),cur);
			quater++;
		}
		if ((int)cur>last) {
			last=(int)cur;
			if(sched_yield()==-1) errExit("sched_yield");
		}
	}
}
int main(int argc,char *argv[]) {
	int prio=50;
	setbuf(stdout,NULL);
	struct sched_param sp;
	sp.sched_priority=prio;
	if (sched_setscheduler(0,SCHED_FIFO,&sp)==-1) errExit("sched_setscheduler");
	switch(fork()) {
		case -1: errExit("fork");
		case 0: consumeCPU(); _exit(EXIT_SUCCESS);
		default: {
            consumeCPU();wait(NULL);break;
            }
	}
	exit(EXIT_SUCCESS);
}
