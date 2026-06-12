#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/uio.h>
#include<stdio.h>
#include<stdlib.h>
#include "../Lib/tlpi_hdr.h"
ssize_t myreadv(int fd,const struct iovec iov[],int iovcnt) {
	int len=0;
	for (int i=0;i<iovcnt;i++) len+=iov[i].iov_len;
	char *buffer=(char*)malloc(len);
	if (buffer==NULL) errExit("buffer");
	int byteRead=0;
	if ((byteRead=read(fd,buffer,len))==-1) errExit("read");
	int cnt=0;
	for (int i=0;i<iovcnt;cnt+=iov[i].iov_len,i++) {
		int numbyte=max(0,min(iov[i].iov_len,byteRead-cnt));
		memcpy(iov[i].iov_base,buffer+cnt,numbyte);
	}
	free(buffer);
	return byteRead;
}
ssize_t mywritev(int fd,const struct iovec iov[],int iovcnt) {
	int len=0;
	for (int i=0;i<iovcnt;i++) len+=iov[i].iov_len;
	char *buffer=(char*)malloc(len);
	if (buffer==NULL) errExit("buffer");
	int cnt=0;
	for (int i=0;i<iovcnt;cnt+=iov[i].iov_len,i++)  {
		memcpy(buffer+cnt,iov[i].iov_base,iov[i].iov_len);
	}
	int byteWriten;
	if ((byteWriten=write(fd,buffer,len))==-1) errExit("write");
	free(buffer);
	return byteWriten;
}
int main(int argc,char *argv[]) {
	if (argc<2) usageErr("%s filename",argv[0]);
	int fd=open(argv[1],O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	if (fd==-1) errExit("open");
	struct stat a;
	int b;
	char c[10];
	struct iovec iov[3];
	iov[0].iov_base=&a;
	iov[0].iov_len=sizeof(a);
	iov[1].iov_base=&b;
	iov[1].iov_len=sizeof(b);
	iov[2].iov_base=c;
	iov[2].iov_len=sizeof(c);
	int writebyte=mywritev(fd,iov,3);
	printf("write success %lld bytes\n",writebyte);
	lseek(fd,0,SEEK_SET);
	int readbyte=myreadv(fd,iov,3);
	printf("read success %lld bytes\n",readbyte );
	exit(EXIT_SUCCESS);
}
