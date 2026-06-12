#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<pwd.h>
#include<grp.h>
#define SG_SIZE (NGROUPS_MAX + 1)
char *getGroupNameFromId(gid_t gid) {
	struct group * grp;
	grp=getgrgid(gid);
	if (grp==NULL) return NULL;
	char *ans=(char*)malloc(strlen(grp->gr_name)+1);
	strcpy(ans,grp->gr_name);
	return ans;
}
char *getUserNameFromId(uid_t uid) {
	struct passwd* pwd;
	pwd=getpwuid(uid);
	if(pwd==NULL) return NULL;
	char *ans=(char*)malloc(strlen(pwd->pw_name)+1);
	strcpy(ans,pwd->pw_name);
	return ans;
}
uid_t getIdFromUserName(char *username) {
	struct passwd*pwd;
	pwd=getpwnam(username);
	if (pwd==NULL) return -1;
	return pwd->pw_uid;
}
gid_t getGroupIdFromUserId(uid_t uid) {
	struct passwd *pwd;
	pwd=getpwuid(uid);
	if (pwd==NULL) return -1;
	return pwd->pw_gid;
}
int myinitgroups(char *user,gid_t group) {
	if (setgid(group)==-1) {
		perror("setgid");
		return -1;
	}
	struct group * grp=getgrgid(group);
	if (grp==NULL) return -1;
	uid_t userid=getIdFromUserName(user);
	if (userid==-1) return -1;
	struct group * tg;
	gid_t grouplist[SG_SIZE];
	int cnt=0;
	grouplist[cnt++]=group;

	while((tg=getgrent())!=NULL) {
		for (char **s=tg->gr_mem;*s!=NULL;s++) if (strcmp(*s,user)==0) grouplist[cnt++]=tg->gr_gid;
		tg++;
	}
	endgrent();
	return setgroups(cnt,grouplist);

}
int main(int argc,char *argv[]) {
	uid_t realuid=getuid();
	char *username=getUserNameFromId(realuid);
	if (username==NULL) {
		perror("error username");
		return 1;
	}
	printf("username: %s (%ld)\n",username,(long)realuid);
	gid_t groupid=getGroupIdFromUserId(realuid);
	if (groupid==-1) {
		perror("error groupid");
		return 1;
	}
	myinitgroups(username,groupid);
}
