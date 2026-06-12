
#include<pwd.h>
#include<grp.h>
#include<ctype.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pwd.h>
//#include "ugid_functions.h"
#include<stdio.h>
//Return name corresponding to 'uid', or Null on error
char *userNameFromId(uid_t uid){
	struct passwd * pwd;
	pwd=getpwuid(uid);
	return (pwd==NULL)?NULL: pwd->pw_name;
}
//Return UID corrensponding to 'name', or -1 on error
uid_t userIdFromName(const char *name) {
	struct passwd *pwd;
	uid_t u;
	char *endptr;
	if (name==NULL||*name=='\0') {
		//On NULL or empty string -> return an error
		return -1;
	}
	u=strtol(name,&endptr,10);//As a convenience to caller allow
	//numberic string
	if (*endptr=='\0') return u;
	pwd=getpwnam(name);
	if (pwd==NULL) return -1;
	return pwd->pw_uid;
}
//Return name corresponding to 'gid', or Null on error
char *groupNameFromId(gid_t gid) {
	struct group *grp;
	grp=getgrgid(gid);
	return (grp==NULL)?NULL : grp->gr_name;
}
//Return GID corrensponding to 'name' or -1 on error
gid_t groupIdFromName(const char *name) {
	struct group *grp;
	gid_t g;
	char *endptr;
	if(name==NULL||*name=='\0') {
		//On null or empty string -> return error
		return -1;
	}
	g=strtol(name,&endptr,10);
	if (*endptr=='\0') return g;
	grp=getgrnam(name);
	if (grp==NULL) return -1;
	return grp->gr_gid;
}
int main(int argc,char *argv[]) {
	if (argc<2) {
		printf("Usage error");
		return 1;
	}
	printf("usedid from name: %s: %d",argv[1],userIdFromName(argv[1]));
}
