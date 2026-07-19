
#define _GNU_SOURCE /*Get '_sys_nerr' and '_sys_errlist'
declarations from <stdio.h>*/
#include<stdio.h>
#include<string.h>//Get declaration of strerror()


// THÊM 2 DÒNG NÀY: Khai báo extern vì các bản glibc mới đã giấu nó đi
extern const char *const _sys_errlist[];
extern int _sys_nerr;

#define MAX_ERROR_LEN 256 /*Maximum length of string
							return by strerror()*/
static char buf[MAX_ERROR_LEN];//Statically allocated return buffer
char *streerror(int err) {
    // Lấy chuỗi lỗi chuẩn từ hệ thống thông qua thư viện <string.h>
    char *sys_err = strerror(err);

    if (sys_err == NULL) {
        snprintf(buf, MAX_ERROR_LEN, "unknown error %d", err);
    } else {
        strncpy(buf, sys_err, MAX_ERROR_LEN - 1);
        buf[MAX_ERROR_LEN - 1] = '\0';
    }
    return buf;
}
