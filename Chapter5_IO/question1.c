
#define _FILE_OFFSET_BITS 64
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/uio.h>
#include<unistd.h>
#include<stdlib.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	if (argc<3||strcmp(argv[1],"--help")==0) {
		usageErr("%s pathname offset");
	}
	int fd=open(argv[1],O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
	if (fd==-1) errExit("open");
	off_t off=atoll(argv[2]);
	if (lseek(fd,off,SEEK_SET)==-1) errExit("lseek");
	if (write(fd,"test",4)==-1) errExit("write");
	exit(EXIT_SUCCESS);
}
