#include <time.h>
#include "log.h"


namespace escpplib{


FILE* g_escpplib_log_stdout = stdout;
FILE* g_escpplib_log_stderr = stderr;

void SetLogStdOutFd(FILE* fd) {
    g_escpplib_log_stdout = fd;
}

void SetLogStdErrFd(FILE* fd) {
    g_escpplib_log_stdout = fd;
}


void getLogTime(char *format) {
	time_t t = time(NULL);
	struct tm time;
	localtime_r(&t, &time);
	strftime( format, 18, "%Y%m%d-%H:%M:%S", &time);
	format[17] = 0;	
}



}// namespace escpplib
