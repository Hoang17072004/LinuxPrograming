#define _GNU_SOURCE /*Get '_sys_nerr' and '_sys_errlist'
declarations from <stdio.h>*/
#include<stdio.h>
#include<string.h> //Get declaration of strerror()
#include<pthread.h>
#define MAX_ERROR_LEN 256 /*Maximum length of string in per-thread
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
static __thread char buf[MAX_ERROR_LEN];//Thread-local return buffer
char *streerror(int err) {
	if (err<0||err>=_sys_nerr||_sys_errlist[err]==NULL) {
		snprintf(buf,MAX_ERROR_LEN,"Unknow error %d",err);
	} else {
		strncpy(buf,_sys_errlist[err],MAX_ERROR_LEN-1);
		buf[MAX_ERROR_LEN-1]='\0';//Ensure null termination
	}
	return buf;
}
