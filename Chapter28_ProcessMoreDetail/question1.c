#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<time.h>
#include<sys/time.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	struct timeval st,fi;
	int n=(argc==1)?1:getInt(argv[1],0,"num");
	int status;
	gettimeofday(&st,NULL);
	for (int i=0;i<n;i++) {
		switch(fork()) {
			case -1: errExit("fork");
			case 0:_exit(EXIT_SUCCESS);
			default: wait(&status);
		}
	}
	gettimeofday(&fi,NULL);
	printf("Fork took time: %.3f s\n",(fi.tv_sec-st.tv_sec)+(fi.tv_usec-st.tv_usec)/1e6);
	gettimeofday(&st,NULL);
	for (int i=0;i<n;i++) {
		switch(vfork()) {
			case -1: errExit("vfork");
			case 0: _exit(EXIT_SUCCESS);
			default: wait(&status);
		}
	}
	gettimeofday(&fi,NULL);
	printf("Vfork took time: %.3f s\n",(fi.tv_sec-st.tv_sec)+(fi.tv_usec-st.tv_usec)/1e6);
}
