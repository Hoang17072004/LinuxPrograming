#define _POSIX_C_SOUCR 199309
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include "curr_time.h"
#define BUF_SIZE 1000
char *currTime(const char *format) {
	static char staticBuf[BUF_SIZE];
	time_t t;
	ssize_t s;
	struct tm*tm;
	t=time(NULL);
	tm=localtime(&t);

	s=strftime(staticBuf,BUF_SIZE,(format!=NULL)?format:"%Y-%m-%d %H:%M:%S",tm);
	return (s==0)?NULL:staticBuf;
}

