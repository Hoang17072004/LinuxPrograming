#include<time.h>
#include<locale.h>
#include "../Lib/tlpi_hdr.h"
#define BUF_SIZE 200
#include<stdio.h>
int main(int argc,char *argv) {
	time_t t;
	struct tm *loc;
	char buf[BUF_SIZE];
	if (setlocale(LC_ALL,"")==NULL)
		errExit("setlocalte");//Use locale settings in conversion
	t=time(NULL);
	printf("ctime() of time() is value: %s",ctime(&t));
	loc=localtime(&t);
	if (loc==NULL) errExit("localtime");
	printf("asctime() of local time is: %s",asctime(loc));
	if (strftime(buf,BUF_SIZE, "%A, %d %B %Y, %H:%M:%S %Z",loc)==0)
		fatal("strftime returned 0");
	printf("strftime() of local time is: %s\n",buf);
	exit(EXIT_SUCCESS);
}
