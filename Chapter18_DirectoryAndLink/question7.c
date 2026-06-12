#define _XOPEN_SOURCE 600
#include<ftw.h>
#include<unistd.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
int cntdir=0,cntreg=0,cntlnk=0,total=0;
int dirTree(const char*pathname,const struct stat*sbuf,int type,struct FTW *ftwb) {
    printf("%*s",4*ftwb->level," ");
	printf("%s\n",&pathname[ftwb->base]);
	switch(sbuf->st_mode&S_IFMT) {
		case S_IFDIR: printf("Directory\n");break;
		case S_IFREG: printf("File regular\n");break;
		case S_IFLNK: printf("Link file\n");break;
		default: printf("?\n");break;
	}
	switch (type) {
        case FTW_D:    // directory before reading
        case FTW_DP:   // directory after reading (with FTW_DEPTH)
        case FTW_DNR:  // directory not readable
            cntdir++;
            break;
        case FTW_F:
            cntreg++;
            break;
        case FTW_SL:   // symlink
        case FTW_SLN:  // symlink that points to non-existent file
            cntlnk++;
            break;
        default:
            break;
    }

	total++;
	if (type!=FTW_NS)printf("Inode number: %7ld\n",(long)sbuf->st_ino);

	return 0;
}
int main(int argc,char *argv[]) {
	if (argc<2) usageErr("%s [dir]",argv[0]);
	if (nftw(argv[1],dirTree,10,FTW_DEPTH)==-1) errExit("nftw");
	printf("Statistic\n");
	printf("Number of directory: %d, Percentage: %.2f\n",cntdir,cntdir/(double)total*100);
	printf("Number of regularfile: %d, Percentage: %.2f\n",cntreg,cntreg/(double)total*100);
	printf("Number of linkfile: %d, Percentage: %.2f\n",cntlnk,cntlnk/(double)total*100);
	exit(EXIT_SUCCESS);
}

