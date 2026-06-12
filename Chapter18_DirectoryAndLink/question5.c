#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<dirent.h>
#include "../Lib/tlpi_hdr.h"
#define  MAX_DEPTH 100
int main(int argc,char *argv[]) {
	struct stat stcur;
	char ** arr=(char **)malloc(MAX_DEPTH*sizeof(char*));
	int len=0;
	arr[len++]=strdup(argv[0]+2);
	while(1) {
		struct dirent* entry;
	 	struct stat st;
		if(stat(".",&st)==-1) errExit("lstat");
		if(chdir("..")==-1) errExit("chdir");
		struct stat pr;
		if (stat(".",&pr)==-1) errExit("stat");
		if (st.st_ino==pr.st_ino&&st.st_dev==pr.st_dev) {
			//Same directory
			break;
		}
		DIR *dir=opendir(".");
		if (dir==NULL) errExit("opendir");
		struct stat cur;
		while((entry=readdir(dir))!=NULL) {
			if (lstat(entry->d_name,&cur)==-1) errExit("lstat");
			if (cur.st_ino==st.st_ino&&cur.st_dev==st.st_dev) {
				arr[len++]=strdup(entry->d_name);
				break;
			}
		}
		closedir(dir);
	}
	printf("/");
	for (int i=len-1;i>=0;i--) {
		printf("%s",arr[i]);
		if (i) printf("/");
	}
	for (int i=0;i<len;i++) free(arr[i]);
	free(arr);
	exit(EXIT_SUCCESS);
}
