
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
//dup(oldfd) triển khai bằng fcntl
int my_dup(int oldfd) {
	return fcntl(oldfd,F_DUPFD,0);
}
//dup2(oldfd,newfd) triền khai bằng fcntl
int my_dup2(int oldfd,int newfd) {
	if (oldfd==newfd) {
		//Kiểm tra oldfd có hợp lệ không
		if (fcntl(oldfd,F_GETFL)==-1) {
			errno=EBADF;
			return -1;
		}
		return newfd;
	}
	//Nếu newfd đang mở thì đóng nó trước
	close(newfd);
	//Duplicating oldfd vào newfd
	int ret=fcntl(oldfd,F_DUPFD,newfd);
	return ret;
}
int main(int argc,char *argv[]) {
    if (argc!=3)  usageErr("%s outfile content");
    int fd=open(argv[1],O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR);
    if (fd==-1) errExit("open");
    if(my_dup2(fd,STDOUT_FILENO)==-1) errExit("mydup2");
    printf("%s\n",argv[2]);
    close(fd);
    exit(EXIT_SUCCESS);

}
