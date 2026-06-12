#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	struct stat sb;
	if (argc<2||strcmp(argv[1],"--help")==0) usageErr("%s filename",argv[1]);
	if (stat(argv[1],&sb)==-1) errExit("stat");
	int newmod=sb.st_mode|(S_IRUSR|S_IRGRP|S_IROTH);
	if (S_ISDIR(sb.st_mode)||(sb.st_mode&(S_IXUSR|S_IXGRP|S_IXOTH))) newmod|=S_IXUSR|S_IXGRP|S_IXOTH;
	if (chmod(argv[1],newmod)==-1) errExit("chmod");
	exit(EXIT_SUCCESS);
}
