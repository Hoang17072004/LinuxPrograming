#define _DEFAULT_SOURCE
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#include <sys/sysmacros.h>
char *filePermStr(mode_t mode,int flags) {
	char *str=(char*)malloc(12);
	str[10]='\0';
	strcpy(str,"-------");
	//Kieu file
	if (S_ISDIR(mode)) str[0]='d';
	else if (S_ISCHR(mode)) str[0]='c';
	else if (S_ISBLK(mode)) str[0]='b';
	else if (S_ISFIFO(mode)) str[0]='p';
	else if (S_ISLNK(mode)) str[0]='l';
	else if (S_ISSOCK(mode)) str[0]='s';
	//Quyen cho owner
	if (mode&S_IRUSR) str[1]='r';
	if (mode&S_IWUSR) str[2]='w';
	if (mode&S_IXUSR)  str[3]='x';
	//Quyen cho group
	if (mode&S_IRGRP) str[4]='r';
	if (mode&S_IWGRP) str[5]='w';
	if (mode&S_IXGRP) str[6]='x';
	//Quyen cho others
	if (mode&S_IROTH) str[7]='r';
	if (mode&S_IWOTH) str[8]='w';
	if (mode&S_IXOTH) str[9]='x';
	return str;
}
static void displayStatInfo(const struct stat *sb) {
	printf("File type:			");
	switch(sb->st_mode&S_IFMT) {
		case S_IFREG: printf("regular file\n");
		case S_IFDIR: printf("directory\n");
		case S_IFCHR: printf("character device\n");
		case S_IFBLK: printf("block device\n");
		case S_IFLNK: printf("symbolic (soft) link\n");
		case S_IFIFO: printf("FIFO  or pipe\n");
		case S_IFSOCK: printf("socket\n");
		default: printf("unknow file type?\n");
	}
	printf("Device contraining i-node: major: %ld	minor:%ld\n",(long)major(sb->st_dev),(long)minor(sb->st_dev));
	printf("I-node number:		%ld\n",(long)sb->st_ino);
	printf("Mode:		%ld (%s)\n",
		(unsigned long)sb->st_mode,filePermStr(sb->st_mode,0));
	if (sb->st_mode&(S_ISUID|S_ISGID|S_ISVTX))
		printf(" 	sprecial bits set: 	%s%s%s\n",
			(sb->st_mode&S_ISUID)?"set-uid ":"",
			(sb->st_mode&S_ISGID)?"set-gid ":" ",
			(sb->st_mode&S_ISVTX)?"sticky ":"");
	printf("Number of (hard) links: %ld\n",(long) sb->st_nlink);
	printf("Ownership: 	UID=%ld 	GID=%ld\n",
		(long)sb->st_uid,(long)sb->st_gid);
	if (S_ISCHR(sb->st_mode)||S_ISBLK(sb->st_mode))
		printf("Device number (st_rdev): major: %ld; minor=%ld\n",
			(long)major(sb->st_rdev),(long)minor(sb->st_rdev));
	printf("File size: 	%lld bytes\n",(long long)sb->st_size);
	printf("Optimal I/O block size:	%ld bytes\n",(long)sb->st_blksize);
	printf("512B blocks allocated: 	%lld\n",(long long)sb->st_blocks);
	printf("Last file access: 	%s",ctime(&sb->st_atime));
	printf("Last file modification: %s",ctime(&sb->st_mtime));
	printf("Last status change: 	%s",ctime(&sb->st_ctime));
}
int main(int argc,char *argv[]) {
	struct stat sb;
	Boolean statLink;//True if  "-l" specified(i.e lstat())
	int fname;//Location of filename argument
	statLink=(argc>1)&&strcmp(argv[1],"-l")==0;
	fname=statLink?2:1;
	if (fname>=argc||(argc>1&&strcmp(argv[1],"--help")==0))
		usageErr("%s 	[-l] file\n",argv[0]);
	if (statLink) {
		if (lstat(argv[fname],&sb)==-1) errExit("lstat");
	} else {
		if (stat(argv[fname],&sb)==-1) errExit("stat");
	}
	displayStatInfo(&sb);
}
