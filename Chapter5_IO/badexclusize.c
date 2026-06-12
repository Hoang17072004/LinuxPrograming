#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#include<errno.h>
//using namespace std;
int main(int argc,char *argv[]) {
	int fd=open(argv[1],O_WRONLY);
	if (fd==-1) {
		printf("[PID %ld] File %s already exist\n",(long)getpid(),argv[1]);
		close(fd);
	} else {
		if (errno!=ENOENT) {//Failed for unexpected reason
			errExit("open");
		} else {
			fd=open(argv[1],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
			if (fd==-1) errExit("open");
			printf("[PID %ld] Created file %s exclusily \n",(long)getpid(),argv[1]);
		}
	}
}
