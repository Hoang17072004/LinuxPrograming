#ifndef ITIMERSPEC_FROM_STR_H
#define ITIMERSPEC_FROM_STR_H
#include<time.h>
/* Chuyển chuỗi dạng "value.sec[/value.nsec][:interval.sec[/interval.nsec]]"
   thành cấu trúc itimerspec.
   Ví dụ: "5:2" -> it_value=5s, it_interval=2s
          "10/500000000:3/100000000" -> it_value=10.5s, it_interval=3.1s */
void itimerspecFromStr(char *str,struct itimerspec *tsp);
#endif
