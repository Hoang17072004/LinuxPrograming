#include<time.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>
#include<fcntl.h>
#include<sys/stat.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	struct timeval st,fi;
	gettimeofday(&st,NULL);
	if (argc!=3) usageErr("%s folder number");
	int num=getInt(argv[2],GN_ANY_BASE,"number");
	int len=strlen(argv[2]);
	const int MAX_LENGTH=100;
	char *path=(char*)malloc(MAX_LENGTH);
	int ids[num];
	for (int i=1;i<=num;i++) {
		char *s=(char*)malloc(len+1);
		int t=i;
		int v=len-1;
		for (int j=0;j<len;j++) s[j]='0';
		s[len]='\0';
		while(t) {
			s[v]=(char)(t%10+'0');
			t/=10;
			v--;
		}
		snprintf(path,MAX_LENGTH,"%s/%s",argv[1],s);
		int fd=open(path,O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR);
		if (write(fd,"k",1)==-1) errExit("write");
		if (fd==-1) errExit("open");
		free(s);
		ids[i-1]=fd;
		close(fd);
	}
	printf("Create success %d file in %s\n",num,argv[1]);
	for (int i=1;i<=num;i++) {
		char *s=(char*)malloc(len+1);
		int t=i;
		int v=len-1;
		for (int j=0;j<len;j++) s[j]='0';
		s[len]='\0';
		while(t) {
			s[v]=(char)(t%10+'0');
			t/=10;
			v--;
		}
		snprintf(path,MAX_LENGTH,"%s/%s",argv[1],s);
		if (unlink(path)==-1) errExit("unlink");
		free(s);
	}
	printf("Delete success %d file in %s\n",num,argv[1]);
	gettimeofday(&fi,NULL);
	printf("Took %.6f seconds",fi.tv_sec-st.tv_sec+(fi.tv_usec-st.tv_usec)/1e6);
}
