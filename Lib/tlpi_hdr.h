#ifndef TLPI_HDR_H
#define TPLI_HDR_H //Prevent accidental double inclusion
#include<sys/types.h>//Type definitions used by many programs
#include<stdlib.h> //Prototypes of commonly used library functins
//plus EXIT_SUCCESS and EXIT_FAILURE constants
#include<unistd.h>//Prototype for many system calls
#include<error.h>//Declares errno and defines error constants
#include<string.h>//Commonly used string-handling functions
#include "get_num.h" //Declares our functions for handling numeric arguments (getInt(),getLong())
#include "error_functions.h" //Declares our error_handling function
typedef enum {FALSE,TRUE} Boolean;
#define min(m,n)((m)<(n)?(m):(n))
#define max(m,n)((m)>(n)?(m):(n))
#endif
