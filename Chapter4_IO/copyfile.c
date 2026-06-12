
//1 số cờ
//openFlags: đây là tập hợp các cờ (flags) truyền cho hàm open() trong C (thư viện POSIX).
//
//O_CREAT: nếu file chưa tồn tại thì tạo mới.
//
//O_WRONLY: mở file chỉ để ghi (write only).
//
//O_TRUNC: nếu file đã tồn tại thì xóa nội dung cũ, ghi đè từ đầu.

//usageErr vừa in ra cách dùng, vừa kết thúc chương trình.
// Nó không chỉ đơn thuần là printf, mà là một hàm tiện ích
// để báo lỗi cú pháp và dừng chương trình.

//filePerms: đây là tập hợp quyền truy cập (permissions) khi tạo file mới.
//Các macro S_IRUSR, S_IWUSR, … nằm trong <sys/stat.h>:
//
//S_IRUSR: quyền đọc cho user (chủ sở hữu).
//
//S_IWUSR: quyền ghi cho user.
//
//S_IRGRP: quyền đọc cho group.
//
//S_IWGRP: quyền ghi cho group.
//
//S_IROTH: quyền đọc cho others (người khác).
//
//S_IWOTH: quyền ghi cho others
//👉 Kết hợp lại: file được tạo sẽ có quyền rw-rw-rw-
// (tức là ai cũng có thể đọc và ghi, nhưng không có quyền
// thực thi).


//Hàm fatal() trong The Linux Programming Interface
// (và nhiều chương trình mẫu trong sách này)
//là một hàm tiện ích để báo lỗi nghiêm trọng rồi
// kết thúc chương trình.
#include<sys/stat.h>
#include<fcntl.h>
#include "../Lib/tlpi_hdr.h"

#ifndef BUF_SIZE //Allow "cc -D " to overide definition
#define BUF_SIZE 1024
#endif
int main(int argc,char *argv[]) {
	int inputFd,outputFd,openFlags;
	mode_t filePerms;
	ssize_t numRead;
	char buf[BUF_SIZE];
	if (argc!=3||strcmp(argv[1],"--help")==0) {
		usageErr("%s old-file new-file\n",argv[0]);
	}
	inputFd=open(argv[1],O_RDONLY);
	if (inputFd==-1) errExit("Open file %s",argv[1]);
	openFlags=O_CREAT|O_WRONLY|O_TRUNC;
	filePerms=S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;//rw-rw-rw
	outputFd=open(argv[2],openFlags,filePerms);
	if (outputFd==-1) errExit("Openning file %s",argv[2]);
	//tranfer data until we encounter end of input or an error
	while((numRead=read(inputFd,buf,BUF_SIZE))>0) {
		if (write(outputFd,buf,numRead)!=numRead) fatal("Couldn't write whole buffer");

	}
	if (numRead==-1) errExit("read");
	if (close(inputFd)==-1) errExit("close input");
	if (close(outputFd)==-1) errExit("close output");
	exit(EXIT_SUCCESS);
}
