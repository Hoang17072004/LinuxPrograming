
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
int main(int argc,char *argv[]) {
	mode_t old=umask(0);
	umask(old);
	printf("Current mask: %o\n",old);
}
