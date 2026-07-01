#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<unistd.h>
extern char **environ;
//Bước 1: gom danh sách đối số thành argv[]
char **build_argv(const char *filename,va_list ap) {
	int count=1;//argv[0]=filename
	const char*arg;
	va_list ap_copy;
	va_copy(ap_copy,ap);
	while((arg=va_arg(ap_copy,const char *))!=NULL) {
		count++;
	}
	va_end(ap_copy);
	char **argv=malloc((count+1)*sizeof(char*));
	if (argv==NULL) return NULL;
	argv[0]=strdup(filename);
	int i=1;
	while((arg=va_arg(ap,const char *))!=NULL) {
		argv[i++]=strdup(arg);
	}
	argv[i]=NULL;//Kết thúc bằng NULL
	return argv;
}
//Bước 2: tìm trong PATH và gọi execve()
int my_execlp(const char *filename,const char *arg,...) {
	va_list ap;
	va_start(ap,arg);
	char **argv=build_argv(filename,ap);
	va_end(ap);
	if (argv==NULL) {
		perror("malloc");
		return -1;
	}
	//Nếu filename có chứa '/' , coi như pathname
	if (strchr(filename,'/')) {
		execve(filename,argv,environ);
		perror("execve");
		return -1;
	}
	//Nếu không có '/' duyệt PATH
	char *path=getenv("PATH");
	if (path==NULL) path="/bin:/usr/bin";
	char *dup=strdup(path);
	char *dir=strtok(dup,":");
	while(dir!=NULL) {
		char fullpath[1024];
		snprintf(fullpath,sizeof(fullpath),"%s/%s",dir,filename);
		execve(fullpath,argv,environ);
		//Nếu execve thất bại,thử tiếp
		dir=strtok(NULL,":");
	}
	perror("execve");
	return -1;
}
//Chương trình chính
int main() {
	//ví dụ: gọi ls -l /etc
	my_execlp("ls","ls","-l","/etc",(char*)NULL);
	return 0;
}
