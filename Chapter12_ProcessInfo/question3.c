#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<errno.h>
#include "../Lib/tlpi_hdr.h"
#include<ctype.h>
const int MAXPATH=100;
const int MAXLENGTH=1024;
int main(int argc,char *argv[]) {
	uid_t useruid=getuid();
	DIR *dir;
	struct dirent *entry;
	dir=opendir("/proc");
	if (dir==NULL) errExit("opendir");
	char *filename=(char*)malloc(MAXPATH);
	char *line=(char*)malloc(MAXLENGTH);
	char path[MAXPATH];
	char useridstr[20];
	sprintf(useridstr,"%d",useruid);
	while((entry=readdir(dir))!=NULL) {
		if (!isdigit(entry->d_name[0])) continue;
//		snprintf(filename,MAXPATH,"/proc/%s/status",entry->d_name);
//		FILE *fp=fopen(filename,"r");
//		if (fp==NULL) continue;
		DIR *subdir;
		struct dirent *subentry;
		snprintf(path,MAXPATH,"/proc/%s/fd",entry->d_name);
		subdir=opendir(path);
		if (!subdir) errExit("subdir");
		while((subentry=readdir(subdir))!=NULL) {
            snprintf(filename,MAXPATH,"%s/%s",path,subentry->d_name);
            ssize_t len= readlink(filename,line,MAXLENGTH-1);
            if (len!=-1) {
                line[len]='\0';
                printf("%s\n",line);
            }
		}
//		fclose(fp);

	}
	free(line);
	free(filename);
	closedir(dir);
}
