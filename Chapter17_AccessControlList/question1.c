
#include<acl/libacl.h>
#include<sys/acl.h>
#include<unistd.h>
#include<stdlib.h>
#include<grp.h>
#include<pwd.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#include<sys/stat.h>
#include<sys/types.h>
char *printPermset(acl_permset_t permset,int canRead,int canWrite,int canExecute) {
	char *str=(char*)malloc(4);
	str[3]='\0';
	int aclPermVal=acl_get_perm(permset,ACL_READ);
	str[0]=((aclPermVal==1)&canRead)?'r':'-';
	aclPermVal=acl_get_perm(permset,ACL_WRITE);
	str[1]=((aclPermVal==1)&canWrite)?'w':'-';
	aclPermVal=acl_get_perm(permset,ACL_EXECUTE);
	str[2]=((aclPermVal==1)&canExecute)?'x':'-';
	return str;
}
int main(int argc,char *argv[]) {
    printf("argc=%d\n",argc);
	if (argc!=4) usageErr("%s [-u/-g] value filename",argv[0]);
	int opt;
	Boolean isUser;
	char *name;
	while((opt=getopt(argc,argv,"u:g"))!=-1) {
		switch(opt) {
			case 'u':isUser=true,name=optarg;break;
			case 'g':isUser=false,name=optarg;break;
			case '?':usageErr("case");
		}
	}
	if (optind+1!=argc) usageErr("out bound");
	char *filename=argv[optind];

	struct stat st;
	printf("name=%s\n",name);
	if (stat(filename,&st)==-1) errExit("stat");
	if (isUser) {
		struct passwd * pwd=getpwnam(name);
		if (pwd==NULL) errExit("getpwnam");
		uid_t uid=pwd->pw_uid;
		acl_type_t type=ACL_TYPE_ACCESS;
		acl_entry_t entry;
		acl_tag_t tag;
		acl_t acl;
		uid_t * uidp;
		acl_permset_t permset;
		acl=acl_get_file(filename,type);
		if (acl==NULL) errExit("acl_get_file",type);
		for (int entryId=ACL_FIRST_ENTRY;;entryId=ACL_NEXT_ENTRY) {

			if (acl_get_entry(acl,entryId,&entry)!=1) break;//errExit("acl_get_entry");
			if (acl_get_tag_type(entry,&tag)==-1) errExit("acl_get_tag_type");
			if (tag==ACL_USER_OBJ) {
				if (st.st_uid==uid) {
					if (acl_get_permset(entry,&permset)==-1) errExit("acl_get_permset");
					printf("acl_user_obj\t%s\n",printPermset(permset,1,1,1));

				}
			} else if (tag==ACL_USER) {
				uidp=acl_get_qualifier(entry);
				if (uidp==NULL) errExit("acl_get_qualifier");
				if(*uidp==uid) {
					if (acl_get_permset(entry,&permset)==-1) errExit("acl_get_permset");
					printf("acl_user\t%d%s\n",*uidp,printPermset(permset,1,1,1));
				}
				if (acl_free(uidp)==-1) errExit("acl_free");
			}
		}
		if (acl_free(acl)==-1) errExit("acl_free");
	} else {
		struct group *grp=getgrnam(name);
		if (grp==NULL) errExit("getgrname");
		gid_t gid=grp->gr_gid;
		acl_t acl;
		acl_type_t type=ACL_TYPE_ACCESS;
		acl_entry_t entry;
		acl_tag_t tag;
		gid_t *gidp;
		acl_permset_t permset;
		acl=acl_get_file(filename,type);
		if (acl==NULL) errExit("acl_get_file");
		Boolean canRead=FALSE,canWrite=FALSE,canExecute=FALSE;
		for (int entryId=ACL_FIRST_ENTRY;;entryId=ACL_NEXT_ENTRY) {
			if (acl_get_entry(acl,entryId,&entry)!=1) break;//errExit("acl_get_entry");
			if (acl_get_tag_type(entry,&tag)==-1) errExit("acl_get_tag_type");

			if (tag==ACL_MASK)  {
				if (acl_get_permset(entry,&permset)==-1) errExit("acl_get_permset");
				int aclPermVal=acl_get_perm(permset,ACL_READ);
				canRead=canRead||(aclPermVal==1);
				aclPermVal=acl_get_perm(permset,ACL_WRITE);
				canWrite=canWrite||(aclPermVal==1);
				aclPermVal=acl_get_perm(permset,ACL_EXECUTE);
				canExecute=canExecute||(aclPermVal==1);
			}
		}
		for (int entryId=ACL_FIRST_ENTRY;;entryId=ACL_NEXT_ENTRY) {
			if (acl_get_entry(acl,entryId,&entry)!=1) break;//errExit("acl_get_entry");
			if (acl_get_tag_type(entry,&tag)==-1) errExit("acl_get_tag_type");
			if (tag==ACL_GROUP_OBJ) {
				if (st.st_gid==gid) {
					if (acl_get_permset(entry,&permset)==-1) errExit("acl_get_permset");
					printf("acl_group_obj\t%s\n",printPermset(permset,canRead,canWrite,canExecute));
				}
			} else if (tag==ACL_GROUP) {
				gidp=acl_get_qualifier(entry);
				if (gidp==NULL) errExit("acl_get_qualifier");
				if (acl_get_permset(entry,&permset)==-1) errExit("acl_get_permset");
				printf("acl_group\t%d\t%s\n",printPermset(permset,canRead,canWrite,canExecute));
				if(acl_free(gidp)==-1) errExit("acl_free");
			}
		}
		if (acl_free(acl)==-1) errExit("acl_free");
	}
	exit(EXIT_SUCCESS);
}
