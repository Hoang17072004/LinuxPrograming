#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include "../Lib/tlpi_hdr.h"
const int BUF_LENGTH=1024;
int main(int argc,char *argv[]) {
	int fd,byteRead=0;
	int outfile;
	int i=0,j=0;
	char buf[BUF_LENGTH];
	if (argc<3||strcmp(argv[1],"--help")==0) {
		usageErr("%s srcfile dstfile",argv[0]);
	}
	fd=open(argv[1],O_RDONLY);
	if (fd==-1) errExit("open srcfile");
	outfile=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	if (outfile==-1) errExit("open dstfile");
	while((byteRead=read(fd,buf,BUF_LENGTH))>0) {
		for (i=0;i<byteRead;i++) {
            int x;
			if (buf[i]=='\0') {
				int j=i;
				while(j<byteRead&&buf[i]==buf[j]) j++;
				if (lseek(outfile,j-i,SEEK_CUR)==-1) errExit("seek");
				i=j-1;
			} else {
				int j=i;
				while(j<byteRead&&buf[j]!='\0') j++;
				if (write(outfile,&buf[i],j-i)==-1) errExit("seek");
				i=j-1;
			}
		}
	}
	close(fd);
}
