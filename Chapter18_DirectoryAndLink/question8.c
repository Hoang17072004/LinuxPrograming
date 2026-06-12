
#define _XOPEN_SOURCE 600
#include<ftw.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/stat.h>
#include "../Lib/tlpi_hdr.h"
#define MAX_PATH 100
#include<dirent.h>
int mynftw(const char *dirpath,int(*func)(const char *pathname,const struct stat *statbuf,int typeflags,struct FTW *ftwbuf),int nopenfd,int flags,int depth) {
	DIR *dir=opendir(dirpath);
//	static int depth=0;
	if (dir==NULL) errExit("opendir");
	struct dirent *entry;
	if (depth>nopenfd) return 0;
	while((entry=readdir(dir))!=NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

		struct stat st;
		char path[MAX_PATH];
		snprintf(path,MAX_PATH-1,"%s/%s",dirpath,entry->d_name);
		if (lstat(path,&st)==-1) return -1;
		int flag=0;
		switch(st.st_mode&S_IFMT) {
			case S_IFREG: flag|=FTW_F;break;
			case S_IFDIR:flag|=FTW_D;break;
			case S_IFLNK: flag|=FTW_SL;break;

		}
		struct FTW ftwbuf;
		ftwbuf.level=depth;
		ftwbuf.base=strlen(path)-strlen(entry->d_name);
		if(flags&FTW_DEPTH)func(path,&st,flag,&ftwbuf);
		if (S_ISDIR(st.st_mode)&&mynftw(path,func,nopenfd,flags,depth+1)==-1) return -1;
	}
	closedir(dir);
	return 0;
}
int dirTree(const char*pathname,const struct stat*sbuf,int type,struct FTW *ftwb) {
	switch(sbuf->st_mode&S_IFMT) {
		case S_IFDIR: printf("Directory\n");break;
		case S_IFREG: printf("File regular\n");break;
		case S_IFLNK: printf("Link file\n");break;
		default: printf("?\n");break;
	}
//	switch(type) {
//		case FTW_D: printf("Directory\n");break;
//		case FTW_F: printf("Normal file\n"); break;
//		case FTW_SL: printf("Symbolic link\n");break;
//		default:printf("?\n");break;
//	}
	if (type!=FTW_NS)printf("Inode number: %7ld\n",(long)sbuf->st_ino);
	printf("%*s",4*ftwb->level," ");
	printf("%s\n",&pathname[ftwb->base]);
	return 0;
}
int main(int argc,char *argv[]) {
	if (argc<2) usageErr("%s [dir]",argv[0]);
	if (mynftw(argv[1],dirTree,10,FTW_DEPTH,0)==-1) errExit("mynftw");
	exit(EXIT_SUCCESS);
}
