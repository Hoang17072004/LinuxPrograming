
#include<stdio.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>
#include<string.h>
struct passwd * mygetpwnam(const char *username) {
	struct passwd *pwd=NULL;
//	struct *passwd=NULL
	setpwent();
	while((pwd=getpwent())!=NULL&&strcmp(pwd->pw_name,username)!=0) {
		break;
	}
	endpwent();
	return pwd;
}
int main(int argc,char *argv[]) {
	if (argc<2) {
		printf("Not enought argument\n");
		return 1;
	}
	struct passwd * ans=mygetpwnam(argv[1]);
	printf("UID of %s: %lld",argv[1],ans!=NULL?ans->pw_uid:-1);
}
