#define _GNU_SOURCE //Get strsignal() declaration from<string.h>
#include<string.h>
#include<sys/wait.h>
#include "print_wait_status.h" //Declaration of printWaitStatus()
//Examine a wait() status using the W* macros
void printWaitStatus(const char *msg,int status) {
	if (msg!=NULL) printf("%s",msg);
	if (WIFEXITED(status)) {
		printf("Child exited, status=%ld\n",WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		printf("child killed by signal %d (%s)",
				WTERMSIG(status),strsignal(WTERMSIG(status)));
#ifdef WCOREDUMP
		if (WCOREDUMP(status)) printf(" (core dumped)");
#endif
		printf("\n");
	} else if (WIFSTOPPED(status)) {
		printf("child stopped by signal %d (%s)\n",WSTOPSIG(status),strsignal(WSTOPSIG(status)));
#ifdef WIFCONTINUED
	} else if (WIFCONTINUED(status)) {
		printf("child continued\n");
#endif
	} else {
		//Should never happen
		printf("what happend to this child? (status=%x)\n",(unsigned int)status);
	}
}
