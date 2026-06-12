#define _XOPEN_SOURCE 600
#include<unistd.h>
#include<ftw.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#include<sys/stat.h>
#include <sys/inotify.h>
#include<limits.h>
int inotifyFd;
static void displayinfo(struct inotify_event *i) {
	printf("wd: %d\n",i->wd);
	if (i->mask&IN_ACCESS) printf("IN_ACCESS ");
	if (i->mask&IN_CREATE) printf("IN_CREATE ");
	if (i->mask&IN_DELETE) printf("IN_DELETE ");
	if (i->mask&IN_MOVE_SELF)  printf("IN_MOVE_SELF ");
	if (i->mask&IN_MOVED_FROM) printf("IN_MOVE_FROM ");
	if (i->mask&IN_OPEN) printf("IN_OPEN ");
	printf("\n");
	if (i->len>0) printf("name=%s\n",i->name);
}
int func(const char *pathname,const struct stat *st,int typeflag,struct FTW*ftw) {

	int wd=inotify_add_watch(inotifyFd,pathname,IN_ALL_EVENTS);
	if (wd==-1) errExit("inotify_add_watch");
	return 0;
}
#define BUF_LEN (10*(sizeof(struct inotify_event)+NAME_MAX+1))
int main(int argc,char *argv[]) {
	if (argc<2) usageErr("%s pathname...");
	inotifyFd=inotify_init();
	if (inotifyFd==-1) errExit("inotify_init");
	for (int i=1;i<argc;i++) {
		if (nftw(argv[i],func,10,FTW_DEPTH)==-1) errExit("nftw");

	}
	char buf[BUF_LEN];
	struct inotify_event *event;
	while(1) {
		ssize_t numRead=read(inotifyFd,buf,BUF_LEN);
		if (numRead==0) fatal("Read 0");
		if (numRead==-1) errExit("read");
		for (char *p=buf;p<buf+numRead;) {
			event=(struct inotify_event *)p;
			displayinfo(event);
			p+=sizeof(struct inotify_event)+event->len;
		}
	}
	exit(EXIT_SUCCESS);
}
