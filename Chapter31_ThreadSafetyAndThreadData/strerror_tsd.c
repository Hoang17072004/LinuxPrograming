#define _GNU_SOURCE /*Get '_sys_nerr' and '_sys_errlist'
declarations from <stdio.h>*/
#include<stdio.h>
#include<string.h> //Get declaration of strerror()
#include<pthread.h>
#include "../Lib/tlpi_hdr.h"
static pthread_once_t once=PTHREAD_ONCE_INIT;
static pthread_key_t strerrorKey;
#define MAX_ERROR_LEN 256 /*Maximum length of string in  per-thread
buffer returned by strerror()*/
// ======================================================================
// TỰ ĐỊNH NGHĨA LẠI MẢNG LỖI ĐỂ GIỮ NGUYÊN CODE CỦA SÁCH
// (Vì glibc bản mới đã xóa bỏ hoàn toàn biến hệ thống này)
static const char *const _sys_errlist[] = {
    [0]  = "Success",
    [1]  = "Operation not permitted",      // EPERM
    [2]  = "No such file or directory",    // ENOENT
    [22] = "Invalid argument",            // EINVAL
};
static const int _sys_nerr = sizeof(_sys_errlist) / sizeof(_sys_errlist[0]);
// ======================================================================
static void destructor(void *buf) {//Free thread-specific data buffer
	free(buf);
}
static void createKey(void ) {//One-time key creation function
	int s;
	/*Allocate a unique thread-specific data key and save the address
	of the destructor for thread-specific data buffers*/
	s=pthread_key_create(&strerrorKey,destructor);
	if (s!=0) errExitEN(s,"pthread_key_create");
}
char *streerror(int err) {
	int s;
	char *buf;
	//Make first caller allocate key for thread-specific data
	s=pthread_once(&once,createKey);
	if (s!=0) errExitEN(s,"pthread_once");
	buf=pthread_getspecific(strerrorKey);
	if (buf==NULL) {
		/*If first call from this thread , allocate
		buffer for thread, and save its location*/
		buf=malloc(MAX_ERROR_LEN);
		if (buf==NULL) errExit("malloc");
		s=pthread_setspecific(strerrorKey,buf);
		if (s!=0) errExitEN(s,"pthread_setspecific");
	}
	if (err<0||err>=_sys_nerr||_sys_errlist[err]==NULL) {
		snprintf(buf,MAX_ERROR_LEN,"Unknow error %d",err);
	} else {
		strncpy(buf,_sys_errlist[err],MAX_ERROR_LEN-1);
		buf[MAX_ERROR_LEN-1]='\0';//Ensure null termination
	}
	return buf;
}
