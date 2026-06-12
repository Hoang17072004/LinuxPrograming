#include<pwd.h>
#include<grp.h>
#include<unistd.h>
#include "../Lib/tlpi_hdr.h"
char *userNameFromUID(uid_t uid) {
	struct passwd * pwd=getpwuid(uid);
	if (pwd==NULL) return NULL;
	char *ans=(char*)malloc(strlen(pwd->pw_name));
	strcpy(ans,pwd->pw_name);
	return ans;
}
char *groupNameFromGID(gid_t gid) {
	struct group  * grp=getgrgid(gid);
	if (grp==NULL) return NULL;
	char *ans=(char *)malloc(strlen(grp->gr_name));
	strcpy(ans,grp->gr_name);
	return ans;
}
uid_t userIdFromName(char *username) {
	struct passwd * pwd=getpwnam(username);
	if (pwd==NULL) return -1;
	return pwd->pw_uid;
}
gid_t groupIdFromName(char *groupname) {
	struct group *grp=getgrnam(groupname);
	if (grp==NULL) return -1;
	return grp->gr_gid;
}
int main(int argc,char *argv[]) {

	uid_t uid;
	gid_t gid;
	int j;
	Boolean errFnd;
	if (argc<3||strcmp(argv[1],"--help")==0) usageErr("%s owner group [file...]\n"
		"	owner or group can be '-',"
		"meaning leave unchanged \n",argv[0]);
	if (strcmp(argv[1],"-")==0) {
		//don't change owner
		uid=-1;
	} else {
		//Turn username into UID
		uid=userIdFromName(argv[1]);
		if(uid==-1) fatal("No such user(%s)",argv[1]);
	}
	if (strcmp(argv[2],"-")==0) {
		//Don't change group
		gid=-1;
	} else {
		gid=groupIdFromName(argv[2]);
		if (gid==-1) fatal("No group user(%s)",argv[1]);
	}
	//Change ownership of all files named in remaining arguments
	errFnd=FALSE;
	for (j=3;j<argc;j++) {
		if (chown(argv[j],uid,gid)==-1) {
			errMsg("chown :%s",argv[j]);
			errFnd=TRUE;
		}
	}
	exit(errFnd?EXIT_FAILURE:EXIT_SUCCESS);
}
