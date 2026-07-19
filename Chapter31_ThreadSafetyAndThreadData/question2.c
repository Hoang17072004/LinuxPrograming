#include<stdio.h>
#include<pthread.h>
#include "../Lib/tlpi_hdr.h"
#define MAX_LEN 256
static pthread_once_t basename_once =PTHREAD_ONCE_INIT;
static pthread_key_t  basename_key;
static pthread_once_t dirname_once=PTHREAD_ONCE_INIT;
static pthread_key_t dirname_key;
static void basenameDestructor(void *buf) {
	free(buf);
}
static void dirnameDestructor(void *buf) {
	free(buf);
}
static void basenameCreateKey(void ) {
	int s;
	s=pthread_key_create(&basename_key,basenameDestructor);
	if (s!=0) errExitEN(s,"pthread_key_create");
}
static void dirnameCreateKey(void) {
	int s;
	s=pthread_key_create(&dirname_key,dirnameDestructor);
	if (s!=0) errExitEN(s,"pthread_key_create");
}
char *mybasename(const char *basename) {
	int s;
	s=pthread_once(&basename_once,basenameCreateKey);
	if (s!=0) errExit("pthread_once");
	char *buf=pthread_getspecific(basename_key);
	if (buf==NULL) {
		buf=(char*)malloc(MAX_LEN);
		if (buf==NULL) errExit("malloc");
		s=pthread_setspecific(basename_key,buf);
		if (s!=0) errExitEN(s,"pthread_setspecific");
	}
	char *temp=strrchr(basename,'/');
	if (temp==NULL) return NULL;
	strncpy(buf,temp+1,MAX_LEN-1);
	buf[MAX_LEN-1]='\0';
	return buf;
}
char *mydirname(const char *dirname) {
	int s;

	s=pthread_once(&dirname_once,dirnameCreateKey);
	if (s!=0) errExitEN(s,"pthread_once");
	char *buf=pthread_getspecific(dirname_key);
	if (buf==NULL) {
		buf=(char*)malloc(MAX_LEN);
		if (buf==NULL) errExit("malloc");
		s=pthread_setspecific(dirname_key,buf);
		if (s!=0) errExitEN(s,"pthread_setspecific");
	}

	strncpy(buf,dirname,MAX_LEN-1);
	buf[MAX_LEN-1]='\0';
	char *temp=strrchr(buf,'/');
	if (temp!=NULL) *temp='\0';
	return buf;

}
void *threadFunc(void *filename) {
	const char *path=(const char*)filename;
	printf("Thread %lu\n",pthread_self());
	printf("Path:%s\n",path);
	printf("Basename: %s\n",mybasename(path));
	printf("Dirname: %s\n",mydirname(path));
	return NULL;
}
int main() {
    setbuf(stdout, NULL);
	pthread_t 	t1,t2,t3;
	pthread_create(&t1,NULL,threadFunc,"/usr/bin/a.txt");
	pthread_create(&t2,NULL,threadFunc,"/home/kali/Desktop/b.sh");
	pthread_create(&t3,NULL,threadFunc,"/var/log/apache/access.log");
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	exit(EXIT_SUCCESS);
}
