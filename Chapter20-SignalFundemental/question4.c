
#include<signal.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
int my_siginterrupt(int sig,int flag) {
	struct sigaction sa;
	//Lấy cấu hình hiện tại
	if (sigaction(sig,NULL,&sa)==-1) return -1;
	if (flag) {
		//Muốn systemcall bị ngắt bởi tín hiệu
		sa.sa_flags^=SA_RESTART;
	} else {
		//Muốn systemcall tự động restart
		sa.sa_flags|=SA_RESTART;
	}
	//Áp dụng lại
	return sigaction(sig,&sa,NULL);
}
