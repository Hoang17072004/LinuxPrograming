#include<sys/xattr.h>
#include "../Lib/tlpi_hdr.h"
#define X_ATTRIBUTE_SIZE 1000
#include<stdio.h>
int main(int argc,char *argv[]) {
	if (argc<4) usageErr("%s filename attributename value",argv[0]);
	if (setxattr(argv[1],argv[2],argv[3],strlen(argv[3]),0)==-1) errExit("setattribute");
	printf("Set success\n");
	char buf[X_ATTRIBUTE_SIZE];
	int len=0;
	if ((len=getxattr(argv[1],argv[2],buf,X_ATTRIBUTE_SIZE))==-1) errExit("getattribute");
	printf("Attribute: %s: %*.s",argv[2],len,buf);
}
