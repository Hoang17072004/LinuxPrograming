#include<unistd.h>
#include<sys/stat.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
#define MAX_PATH 500
int main(int argc,char *argv[]) {
	if (argc<2) usageErr("%s filename",argv[0]);
	char *path=(char *)malloc(MAX_PATH);
	if (getcwd(path,MAX_PATH)==NULL) errExit("getcwd");
	for (int i=1;i<argc;i++) {

		char *token;
		token=strtok(argv[i],"/");
		char *cur=(char*)malloc(MAX_PATH);
		int len=0;
		while(token!=NULL) {
			if (strcmp(token,".")==0) {
				if (!len) strcpy(cur,path);
//				else strcpy(cur,"/");
				printf("vao cham\n");
			} else if (strcmp(token,"..")==0){
                printf("vao cham cham\n");
				char *last=strrchr(cur,'/');
				if (last!=NULL) {
					int t=last-cur;
					memcpy(cur,cur,t);
					cur[t]='\0';
				} else {
					last=strrchr(path,'/');
					if (last!=NULL) {
						int t=last-path;
						memcpy(cur,path,t);
						cur[t]='\0';
					}
				}
			} else {
                printf("bin thuong\n");
                if (argv[i][0]!='/'&&!len) strcpy(cur,path);
//				int t=len(argv[i])+1;
				strcat(cur,"/");
				strcat(cur,token);

			}
			token=strtok(NULL,"/");
			len=strlen(cur);
			printf("aff: %s\n,len=%d",cur,len);
		}
		printf("Realpath: %s\n",cur);
		free(cur);

	}
	free(path);
}
