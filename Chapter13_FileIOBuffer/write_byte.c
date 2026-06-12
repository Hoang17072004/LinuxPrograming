#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	if (argc!=4||strcmp(argv[1],"--help")==0)
		usageErr("%s file num-byte buf-size\n");
	int fd=open(argv[1],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
	if (fd==-1) errExit("open");
	int numbyte=getInt(argv[2],GN_ANY_BASE,"numbyte");
	int bufsize=getInt(argv[3],GN_GT_0,"bufsize");
	char *buf=(char*)malloc(bufsize);
	for (int i=0;i<=numbyte;i+=bufsize) {
		int v=min(bufsize,numbyte-i);
		if (write(fd,buf,v)==-1) errExit("write");
	}
	free(buf);
	close(fd);
	printf("write %d byte success\n",numbyte);
	exit(EXIT_SUCCESS);

}
