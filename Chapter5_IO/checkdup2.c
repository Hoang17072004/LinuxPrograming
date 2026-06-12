
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	int fd=open(argv[1],O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR);
	if (fd==-1) errExit("open");
	int fd1=dup(fd);
	if (fd1==-1) errExit("dup");
	if(dup2(fd,fd1)==-1) errExit("dup2");
	if(write(fd,"text",4)==-1) errExit("write");
	off_t offset=lseek(fd1,0,SEEK_CUR);
	if (offset==-1) errExit("lseek");
	printf("offset: %lld\n",(long long)offset);
	//Bật cờ O_APPEND trên fd
	int flags=fcntl(fd,F_GETFL);
	fcntl(fd,F_SETFL,flags|O_APPEND);
	int flags2=fcntl(fd1,F_GETFL);
	printf("flags 2 co append: %s \n",(flags2&O_APPEND)?"YES":"NO");
	close(fd);close(fd1);
	exit(EXIT_SUCCESS);
}
