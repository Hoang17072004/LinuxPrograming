
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
extern char **environ;
int mysetenv(const char *name,const char *value,int overwrite) {
	char *pre=getenv(name);
	if (pre!=NULL&&overwrite) return 0;
	int len=strlen(name)+strlen(value)+2;
	char *ope=(char*)malloc(len);
	snprintf(ope,len,"%s=%s",name,value);

	return putenv(ope);
}
int myunsetenv(const char *name) {
	int len=strlen(name);
	char **ep=environ;
	while(*ep!=NULL) {
		if (strncmp(*ep,name,len)==0&&(*ep)[len]=='=') {
			char **d=ep;
			while(*d!=NULL) {
                *d=*(d+1);
//                if (*d==NULL) break;
                d++;
             }
		} else ep++;
	}
	return 0;
}
int main() {
	mysetenv("TESTVAR","hello",1);
	printf("TESTVAR=%s\n",getenv("TESTVAR"));
	myunsetenv("TESTVAR");
	printf("TESTVAR=%s\n",getenv("TESTVAR"));
}
