#include<pthread.h>
#include "../Lib/tlpi_hdr.h"
#include<stdio.h>
static void *threadFunc(void *arg) {
	int j;
	printf("New thread started\n");//May be a cancellation point
	for (j=1;;j++) {
		printf("Loop %d \n",j);//May be a cancallation point
		sleep(1);//A cancellation point
	}
	//NOTREACHED
	return NULL;
}
int main(int argc,char *argv[]) {
	pthread_t thr;
	int s;
	s=pthread_create(&thr,NULL,threadFunc,NULL);
	if (s!=0) errExitEN(s,"pthread_create");
	void *res;
	sleep(3);
	s=pthread_cancel(thr);
	if (s!=0) errExitEN(s,"pthread_cancel");
	s=pthread_join(thr,&res);
	if (s!=0) errExitEN(s,"pthread_join");
	if (res==PTHREAD_CANCELED) printf("Thread was cancelled\n");
	else printf("Thread was not canceled (should not happen!)\n");
	exit(EXIT_SUCCESS);
}
