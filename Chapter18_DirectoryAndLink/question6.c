#define _XOPEN_SOURCE 600
#include<ftw.h>
#include<unistd.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
int dirTree(const char*pathname,const struct stat*sbuf,int type,struct FTW *ftwb) {
    printf("%*s",4*ftwb->level," ");
	printf("%s\n",&pathname[ftwb->base]);
	switch(sbuf->st_mode) {
		case S_IFDIR: printf("Directory\n");break;
		case S_IFREG: printf("File regular\n");break;
		case S_IFLNK: printf("Link file\n");break;
		default: printf("?\n");break;
	}
	switch(type) {
		case FTW_D: printf("Directory\n");break;
		case FTW_F: printf("Normal file\n"); break;
		case FTW_SL: printf("Symbolic link\n");break;
		default:printf("?\n");break;
	}
	if (type!=FTW_NS)printf("Inode number: %7ld\n",(long)sbuf->st_ino);

	return 0;
}
int main(int argc,char *argv[]) {
	if (argc<2) usageErr("%s [dir]",argv[0]);
	if (nftw(argv[1],dirTree,10,FTW_DEPTH)==-1) errExit("nftw");
	exit(EXIT_SUCCESS);
}
