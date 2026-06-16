#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
void my_abort(void) {
	fflush(NULL);//flush stdio streams
	raise(SIGABRT);//gửi SIGABRT
	//Nếu tiến trình chưa chết (handler trả về)
	signal(SIGABRT,SIG_DFL);//đặt lại về mặc định
	raise(SIGABRT);//gửi lại lần nữa
	_exit(1);//fallback: thoát ngay
}

