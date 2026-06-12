#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
//#include<group.h>
//#include<passwd.h>
int main(int argc,char *argv[]) {
	if (argc<3) usageErr("%s filename mode",argv[1]);
	int mode=getInt(argv[2],GN_ANY_BASE,"num");
	uid_t uid=getuid();
	gid_t gid=getgid();
	struct stat sb;
	if (stat(argv[1],&sb)==-1) errExit("stat");
	int flag=sb.st_mode;
	bool isread=0,iswrite=0,isexecute=0;
	gid_t groups[50];
	int len=0;
	if ((len=getgroups(50,groups))==-1) errExit("getgroup");
	if (uid==0) {
		if (mode&R_OK) isread=1;
		if (mode&W_OK) iswrite=1;
		if (mode&X_OK) isexecute=1;
	}
	else if (uid==sb.st_uid) {
		if (mode&R_OK) isread=1;
		if (mode&W_OK) iswrite=1;
		if (mode&X_OK) isexecute=1;
	}
	else if (gid==sb.st_gid&&memchr(groups,sb.st_gid,len*sizeof(gid_t))) {
		if (mode&R_OK) isread=1;
		if (mode&W_OK) iswrite=1;
		if (mode&X_OK) isexecute=1;
	}
	if (isread) printf("Can read\n");
	if (iswrite) printf("Can write\n");
	if (isexecute) printf("Can execute\n");
}
