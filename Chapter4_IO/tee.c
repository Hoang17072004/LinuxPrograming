#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "../Lib/tlpi_hdr.h"
const int BUF_SIZE=1024;
int main(int argc,char *argv[]) {
	int opt=0;
	int append=0;
	int fd=0;
	int flag=O_WRONLY|O_CREAT;
	char buf[BUF_SIZE];
	int byteRead=0;

	//duyệt qua các option
	while((opt=getopt(argc,argv,"a"))!=-1) {
		if (opt=='a') append=1;//có -a
	}
	if (append) flag|=O_APPEND;
	//sau vòng lặp,optind trỏ tới tên file
	if (optind>=argc) {
		errExit("Usage: %s [-a] file\n",argv[0]);
	}
	fd=open(argv[optind],flag,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	if (fd==-1) errExit("open");
	while((byteRead=read(STDIN_FILENO,buf,BUF_SIZE))>0) {
		if(write(fd,buf,byteRead)==-1) errExit("Write");
	}
	close(fd);
}
