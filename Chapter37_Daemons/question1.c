#include<syslog.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
int main(int argc,char *argv[]) {
	if (argc<2) usageErr("%s message\n",argv[0]);
	int idx=0;
	int level=0;
	if (argv[1][0]=='-') {
		switch(argv[1][1]) {
			case 'e':level=LOG_ERR;break;
			case 'w':level=LOG_WARNING;break;
			case 'd':level=LOG_DEBUG;break;
			default: level=LOG_INFO;
		}
		idx++;
	}
	char *msg=argv[idx];
	openlog(msg,LOG_PID|LOG_CONS|LOG_NOWAIT,LOG_LOCAL0);
	syslog(LOG_INFO,"Message: %s",argv[1]);
	syslog(LOG_LOCAL0|LOG_INFO,"Exiting");
	closelog();
	exit(EXIT_SUCCESS);
}
