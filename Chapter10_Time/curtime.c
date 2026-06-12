#include <time.h>
#define BUF_SIZE 1000
/*Return a string containing the current time formated
according to the specification in 'format' (see strtime(3)
for specifiers).
if 'format' is NULL , we use "5sc" as a specifier (which gives the
 date and time as for ctime(3), but without the trailing newline).
 Returns NULL on error*/
char *currTime(const char *format) {
	static char buf[BUF_SIZE];//Nonreentrant
	time_t t;
	size_t s;
	t=time(NULL);
	struct tm *tm;
	tm=localtime(&t);
	if (tm==NULL) return NULL;
	s=strftime(buf,BUF_SIZE,(format!=NULL)?format:"%c",tm);
	return (s==0)?NULL: buf;
}
