
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/uio.h>
#include<stdlib.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	if (argc<3||strcmp(argv[1],"--help")==0) usageErr("%s filename text");
	int fd=open(argv[1],O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR);
	if (fd==-1) errExit("open");
	if(lseek(fd,0,SEEK_SET)==-1) errExit("lseek");
	if (write(fd,argv[2],strlen(argv[2]))==-1) errExit("write");
	exit(EXIT_SUCCESS);
}
