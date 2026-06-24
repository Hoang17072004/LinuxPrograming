#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#include<fcntl.h>
#include<sys/wait.h>
int main(int argc,char *argv[]) {
	int fd;
	char template[]="/tmp/testXXXXXX";
	setbuf(stdout,NULL);
	fd=mkstemp(template);
	if (fd==-1) errExit("mkstemp");
	switch(fork()) {
		case -1: errExit("fork");
		case 0:
			close(fd);
			printf("Child close\n");
			_exit(EXIT_SUCCESS);
		default:
			printf("Wait child process\n");
			wait(NULL);
			printf("Parent add append and write file\n");
			int flags=fcntl(fd,F_GETFL);
			if (flags==-1) errExit("fcntl");
			flags|=O_APPEND;
			if (fcntl(fd,F_SETFL,flags)==-1) errExit("fcntl");
			write(fd,"hello\n",6);
			if (lseek(fd,0,SEEK_SET)==-1) errExit("seekset");
			char buf[10];
			int byteRead=0;
			if ((byteRead=read(fd,buf,10))==-1) errExit("read");
			buf[byteRead]='\0';
			printf("Written: %s\n",buf);
			exit(EXIT_SUCCESS);

	}
}
