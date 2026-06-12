
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/uio.h>
#include<stdlib.h>
#include "../Lib/tlpi_hdr.h"
#include<stdio.h>
int main(int argc,char *argv[]) {
	if (argc<3) usageErr("%s filename numbyte [x]",argv[0]);
	int fd=open(argv[1],O_CREAT|O_WRONLY|(argc==3?O_APPEND:0),S_IRUSR|S_IWUSR);
	if (fd==-1) errExit("open");
	long long num=atoll(argv[2]);
	for (long long i=0;i<num;i++) {
		if (argc==4) if(lseek(fd,0,SEEK_END)==1) errExit("lseek");
		if(write(fd,"x",1)==-1) errExit("write");
	}
	printf("Write success %lld byte",num);
	exit(EXIT_SUCCESS);
}
