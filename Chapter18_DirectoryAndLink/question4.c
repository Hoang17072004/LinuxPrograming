#include<unistd.h>
#include<dirent.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#include <sys/stat.h>
int main(int argc,char *argv[]) {
	if (argc<2) usageErr("%s [dirname]",argv[0]);
	struct stat st;
	if (stat(argv[1],&st)==-1)errExit("stat");
	if (!S_ISDIR(st.st_mode)) errExit("need a dir");
	struct dirent entry;
	struct dirent *result;
	DIR * dir=opendir(argv[1]);
	if (dir==NULL) errExit("dir");
	Boolean iscurrent=strcmp(argv[1],".")==0;
	while(readdir_r(dir,&entry,&result)==0&&result!=NULL) {
		if (!iscurrent) printf("%s/",argv[1]);
		printf("%s\n",entry.d_name);
	}
	exit(EXIT_SUCCESS);
}
