#include<pthread.h>
#include<stdio.h>
#include<stdint.h>
#include "../Lib/tlpi_hdr.h"
static pthread_cond_t threadDied=PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex=PTHREAD_MUTEX_INITIALIZER;
//Protects all of the following global variables
static int totThreads=0;//Total number of threads creates
static int numLive=0;//Total number of threads still alive or terminated but not yet joined
static int numUnjoinded=0;//Number of terminated threads that have not yet been joined
enum tstate { //Thread states
	TS_ALIVE,//Thread is alive
	TS_TERMINATED,//Thread terminated ,not yet joined
	TS_JOINED //Thread terminated, and joined
};
static struct { //Info about each thread
	pthread_t tid;//ID of this thread
	enum tstate state;//Thread state (TS_* constant above)
	int sleepTime;//Number seconds to live before terminating
} *thread;
static void* threadFunc(void *arg) {//Start function for thread
    // Ép ngược trực tiếp từ void* về int (Không dùng dấu * giải con trỏ)
    int idx = (int)(uintptr_t)arg;
	int s;
	sleep(thread[idx].sleepTime);//Simulate doing somework
	printf("Thread %d terminating\n",idx);
	s=pthread_mutex_lock(&threadMutex);
	if (s!=0) errExitEN(s,"pthread_mutex_lock");
	numUnjoinded++;
	thread[idx].state=TS_TERMINATED;
	s=pthread_mutex_unlock(&threadMutex);
	if (s!=0) errExitEN(s,"pthread_mutex_unlock");
	s=pthread_cond_signal(&threadDied);
	if (s!=0) errExitEN(s,"pthread_cond_signal");
	return NULL;
}
int main(int argc,char *argv[]) {
	int s,idx;
	if (argc<2||strcmp(argv[1],"--help")==0)
		usageErr("%s nsecs...\n",argv[0]);
	thread=calloc(argc-1,sizeof(*thread));
	if (thread==NULL) errExit("calloc");
	//Create all thread
	for ( idx=0;idx<argc-1;idx++) {
        int t=idx;
		thread[idx].sleepTime=getInt(argv[idx+1],GN_NONNEG,NULL);
		thread[idx].state=TS_ALIVE;
		s=pthread_create(&thread[idx].tid,NULL,threadFunc,(void *)(uintptr_t)idx);
		if (s!=0) errExitEN(s,"pthread_create");
	}
	totThreads=argc-1;
	numLive=totThreads;
	//Join with terminated threads
	while(numLive>0) {
		s=pthread_mutex_lock(&threadMutex);
		if (s!=0) errExitEN(s,"pthread_mutex_lock");
		while(numUnjoinded==0) {
			s=pthread_cond_wait(&threadDied,&threadMutex);
			if (s!=0) errExitEN(s,"pthread_mutex_wait");
		}
		for (int idx=0;idx<totThreads;idx++) {
			if (thread[idx].state==TS_TERMINATED) {
				s=pthread_join(thread[idx].tid,NULL);
				if (s!=0) errExitEN(s,"pthread_join");
				thread[idx].state=TS_JOINED;
				numLive--;
				numUnjoinded--;
				printf("Reaped thread %d (numLive=%d)\n",idx,numLive);

			}
		}
		s=pthread_mutex_unlock(&threadMutex);
		if (s!=0) errExitEN(s,"pthread_mutex_unlock");
	}
	exit(EXIT_SUCCESS);
}
