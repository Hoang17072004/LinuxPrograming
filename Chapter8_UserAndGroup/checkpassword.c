
#define _BSD_SOURCE //Get getpass() declaration from <unistd.h>
#define _XOPEN_SOURCE //Get crypt() declaration from <unistd.h>
#include<unistd.h>
#include<stdio.h>
#include<limits.h>
#include<pwd.h>
#include<shadow.h>
#include<errno.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	char *username,*password,*encrypted,*p;
	struct passwd *pwd;
	struct spwd *spwd;
	Boolean authOk;
	size_t len;
	long lnmax;
	lnmax=sysconf(_SC_LOGIN_NAME_MAX);
	if (lnmax==-1) {
		//if limit khong xac dinh,doan dai di
		lnmax=256;
	}
	username=malloc(lnmax);
	if (username==NULL) errExit("malloc");
	printf("Username: ");
	if(fgets(username,lnmax,stdin)==NULL)
		exit(EXIT_FAILURE);//Exit on EOF
	len=strlen(username);
	if (username[len-1]=='\n') username[len-1]='\0';
	pwd=getpwnam(username);
	if (pwd==NULL) fatal("couldn't get password correct");
	spwd = getspnam(username);
    if (spwd == NULL) {
        if (errno == EACCES || errno == EPERM)
            fatal("No permission to read shadow password file (try running with sudo)");
        else
            fatal("Could not retrieve shadow password entry (user might not exist)");
    }
	if (spwd!=NULL) pwd->pw_passwd=spwd->sp_pwdp;//Use the shadow password
	password=getpass("Password: ");
	//Encrypt password and erase cleartext verion immediately
	encrypted=crypt(password,pwd->pw_passwd) ;
	for (p=password; *p!='\0';) *p++='\0';
	if (encrypted==NULL) errExit("crypt");
	// Thêm 2 dòng này để debug:
    printf("Mật khẩu gốc trong shadow: %s\n", pwd->pw_passwd);
    printf("Mật khẩu sau khi crypt() : %s\n", encrypted);
	authOk=strcmp(encrypted,pwd->pw_passwd)==0;
	if (!authOk) {
		printf("Incorrect password\n");
		exit(EXIT_FAILURE);
	}
	printf("Successfully authentication: UID=%ld\n",(long)pwd->pw_uid);
	//Now do authenticated work
	exit(EXIT_SUCCESS);
}
