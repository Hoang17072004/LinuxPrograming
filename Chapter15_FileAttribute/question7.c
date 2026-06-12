
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/fs.h>
#include<unistd.h>
#include<string.h>
int main(int argc,char *argv[]) {
	if (argc<3) {
		fprintf(stderr,"Usage: %s [+i|-i] file...\n");
		exit(EXIT_FAILURE);
	}
	int set=(argv[1][0]=='+') ;
	char flag=argv[1][1];
	for (int i=2;i<argc;i++) {
		int fd=open(argv[i],O_RDONLY);
		if (fd==-1) {
			perror("open");continue;
		}

		int flags;
		if (ioctl(fd,FS_IOC_GETFLAGS,&flags)==-1) {
			perror("getflags");
			close(fd);
			continue;
		}
		if (set&&flag=='i') flags|=FS_IMMUTABLE_FL;
		else if (!set&&flag=='i') flags&=~FS_IMMUTABLE_FL;
		if (ioctl(fd,FS_IOC_SETFLAGS,&flags)==-1) perror("setflags");
		close(fd);
	}
	return 0;
}
