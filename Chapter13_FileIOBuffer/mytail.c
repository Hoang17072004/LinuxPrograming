#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
    printf("tren\n");
	if (argc<2||strcmp(argv[1],"--help")==0)
		usageErr("%s [-n num]",argv[1]);
    int opt;
	int num=5;
	printf("con me mi");
	while((opt=getopt(argc,argv,"n:"))!=-1) {
		switch (opt){
            case 'n':
				num=getInt(optarg,GN_ANY_BASE,"n");
				break;
			default:
				num=5;
				break;
		}

	}
	if (optind>=argc) errExit("optind");
 	printf("toi day");
	int fd=open(argv[optind],O_RDONLY);
	if (fd==-1) errExit("open");
	off_t filesize;
	if ((filesize=lseek(fd,0,SEEK_END))==-1) errExit("lseek");
	int cnt=0;
	ssize_t byteRead=0;
	int bufsize=1024;
	char *buf=(char *)malloc(bufsize);
	char **lines=malloc(num*sizeof(char *));
	int len=0;
	char *tempblock=(char *)malloc(1024*10);
	off_t pos=filesize;
//	int idx[line+1];
	while(cnt<num) {
        off_t t=min(bufsize,pos);
        if(lseek(fd,-t,SEEK_CUR)==-1) errExit("lseek");
        pos-=t;
        byteRead=read(fd,buf,bufsize);
        if (byteRead<=0) break;
		for (int i=0;i<byteRead;i++){
			 tempblock[len++]=buf[i];
			 if (buf[i]=='\n') {
				lines[cnt]=malloc(len+1);
				lines[cnt][len]='\0';
				len=0;
				strcpy(lines[cnt],tempblock);

				strcpy(tempblock,"");
				cnt++;
				if (cnt>=num) break;
//				break;
			}
		}
		if (cnt>=num) break;

	}
	printf("Chay het");
	for (int i=cnt-1;i>=0&&lines[i]!=NULL;i--) printf("%s",lines[i]);

	free(tempblock);
	for (int i=0;i< num;i++) free(lines[i]);
	free(lines);
	free(buf);
}
