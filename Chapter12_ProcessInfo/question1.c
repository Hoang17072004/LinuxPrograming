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
	char useridstr[20];
	sprintf(useridstr,"%d",useruid);
	while((entry=readdir(dir))!=NULL) {
		if (!isdigit(entry->d_name[0])) continue;
		snprintf(filename,MAXPATH,"/proc/%s/status",entry->d_name);
		FILE *fp=fopen(filename,"r");
		if (fp==NULL) continue;
		while(fgets(line,MAXLENGTH,fp)!=NULL) {
			if (strstr(line,"Uid:")!=NULL) {
				char *token=strtok(line," \t");
				token=strtok(NULL," \t");
				if (token&&strcmp(token,useridstr)==0) {
					printf("%s ",entry->d_name);
				}
				break;
			}
		}
		fclose(fp);

	}
	free(line);
	free(filename);
	closedir(dir);
}
