#include<pthread.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
static int glob=0;
struct data{
	pthread_t tid;
	int numloop;
};
static void *threadFunc(void *arg) {
	struct data *t=((struct data*)arg);
	int loops=t->numloop;
	pthread_t tid=t->tid;
	int loc,j;
	for (j=0;j<loops;j++) {
		loc=glob;
		loc++;
		glob=loc;
		printf("Thread %lu: glob=%d\n", (unsigned long)t->tid, glob);
	}
	return NULL;
}
int main(int argc,char *argv[]) {
//	pthread_t t1,t2;
//	int loops1,loops2,s;
	struct data dt1,dt2;
	int s;
	dt1.numloop=(argc>1)?getInt(argv[1],GN_GT_0,"loop1"):1000000;
	dt2.numloop=(argc>2)?getInt(argv[2],GN_GT_0,"loop2"):1000000;
//	struct data dt1,dt2;
	s=pthread_create(&dt1.tid,NULL,threadFunc,&dt1);
	if (s!=0) errExitEN(s,"pthread_create");
	s=pthread_create(&dt2.tid,NULL,threadFunc,&dt2);
	s=pthread_join(dt1.tid,NULL);
	if (s!=0) errExitEN(s,"pthread_join");
	s=pthread_join(dt2.tid,NULL);
	if (s!=0) errExitEN(s,"pthread_join");
	printf("glob=%d\n",glob);
	exit(EXIT_SUCCESS);

}
