#define _GNU_SOURCE
#include<unistd.h>
#include<sys/fsuid.h>
#include "limits.h"
#include<stdio.h>
#include<stdlib.h>
#include<pwd.h>
#include<errno.h>
#include<grp.h>
#include "../Lib/tlpi_hdr.h"
#define SG_SIZE (NGROUPS_MAX + 1)
char* userNameFromID(uid_t uid) {
	struct passwd * pwd;
	pwd=getpwuid(uid);
	char *ans=NULL;
	if (pwd!=NULL) {
		ans=(char*)malloc(strlen(pwd->pw_name)+1);
		strcpy(ans,pwd->pw_name);
	}
	return ans;
}
char *groupNameFromID(gid_t gid) {
	struct group * grp;
	grp=getgrgid(gid);
	char *ans=NULL;
	if (grp!=NULL) {
		ans=(char*)malloc(strlen(grp->gr_name)+1);
		strcpy(ans,grp->gr_name);
	}
	return ans;
}
int main(int argc,char *argv) {
	uid_t ruid,euid,suid,fsuid;
	gid_t rgid,egid,sgid,fsgid;
	gid_t suppGroups[SG_SIZE];
	int numGroups,j;
	char * p;
	if (getresuid(&ruid,&euid,&suid)==-1) errExit("getresuid");
	if (getresgid(&rgid,&egid,&sgid)==-1) errExit("getresgid");
	/*Attemp to change the file-system IDs are always ignored
	for unprivileged processes, but even so, the following calls return
	the current file-system IDs
	*/
	fsuid=setfsuid(0);
	fsgid=setfsgid(0);
	printf("UID: ");
	p=userNameFromID(ruid);
	printf("read=%s (%ld); ",(p==NULL)?"???":p,(long)ruid);
	p=userNameFromID(euid);
	printf("eff=%s (%ld);",(p==NULL)?"???":p,(long)euid);
	p=userNameFromID(suid);
	printf("saved=$s (%ld);",(p==NULL)?"???":p,(long)suid);
	p=userNameFromID(fsuid);
	printf("fs=%s (%ld);",(p==NULL)?"???":p,(long)fsuid);
	printf("\n");
	printf("GID: ");
	p=groupNameFromID(rgid);
	printf("real=%s (%ld);",(p==NULL)?"???":p,(long)rgid);
	p=groupNameFromID(egid);
	printf("eff=%s (%ld);",(p==NULL)?"???":p,(long)egid);
	p=groupNameFromID(sgid);
	printf("save=%s (%ld);",(p==NULL)?"???":p,(long)sgid);
	p=groupNameFromID(fsgid);
	printf("fs=%s (%ld);",(p==NULL)?"???":p,(long)fsgid);
	numGroups=getgroups(SG_SIZE,suppGroups);
	if (numGroups==-1) errExit("getgroups");
	printf("Supplementary groups (%d): ",numGroups);
	for (j=0;j<numGroups;j++) {
		p=groupNameFromID(suppGroups[j]);
		printf("%s (%ld) ",(p==NULL)?"???":p,(long)suppGroups[j]);
	}
	printf("\n");
	exit(EXIT_SUCCESS);
}
