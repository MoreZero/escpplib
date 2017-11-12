#include <time.h>
#include "log/log.h"


namespace escpplib{

void getLogTime(char *format)
{
	time_t t = time(NULL);
	struct tm time;
	localtime_r(&t, &time);
	strftime( format, 18, "%Y%m%d-%H:%M:%S", &time);
	format[17] = 0;	
}



}// namespace escpplib
