#include<sys/stat.h>
#include<signal.h>
#include "become_daemon.h"
#include "../Lib/tlpi_hdr.h"
static const char *LOG_FILE="/tmp/ds.log";
static const char *CONFIG_FILE="/tmp/ds.conf";
/*Definitions of logMessage(), logOpen(), logClose(),and 
readConfigFile() are omitted from this listing*/
static volatile sig_atomic_t hupReceived=0;//Set nonzero on receipt of SIGHUP
static void sighupHandler(int sig) {
	hupReceived=1;
}
int main(int argc,char *argv[]) {
	const int SLEEP_TIME=15;//Time to sleep between messages
	int count=0;//Number of completed SLEEP_TIME intervals
	int unslept;//Time remaining in sleep interval
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=SA_RESTART;
	sa.sa_handler=sighupHandler;
	if (sigaction(SIGHUP,&sa,NULL)==-1) errExit("sigaction");
	if (becomDaemon(0)==-1) errExit("becomeDaemon");
	lopOpen(LOG_FILE);
	readConfigFile(CONFIG_FILE);
	unslept=SLEEP_TIME;
	for (;;) {
		unslept=sleep(unslept);//Return >0 if interupted
		if (hupReceived) {
			//If we got signal
			logClose();
			logOpen(LOG)FILE);
			readConfigFile(CONFIG_FILE);
			hupReceived=0;//Get ready for next sighup
		}
		if (unslept==0) {
			//On completed interval
			count++;
			logMessage("Main: %d",count);
			unslept=SLEEP_TIME;//Reset interval
		}
	}
	
}
