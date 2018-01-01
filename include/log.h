#ifndef ESCPPLIB_LOG_H
#define ESCPPLIB_LOG_H


#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

namespace escpplib{
extern void getLogTime(char *format);

extern FILE* g_escpplib_log_stdout;
extern FILE* g_escpplib_log_stderr;


extern void SetLogStdOutFd(FILE* fd);
extern void SetLogStdErrFd(FILE* fd);

}// namespace escpplib

#define LOG_INFO(format, args...) \
	do{\
		char formattime[18];\
		escpplib::getLogTime(formattime);\
		fprintf(escpplib::g_escpplib_log_stdout,"[INFO][%s][%s:%d#%s]" format "\n", formattime, __FILE__, __LINE__, __FUNCTION__, ##args);\
	}while(0)



#define LOG_DEBUG(format, args...) \
	do{\
		char formattime[18];\
		escpplib::getLogTime(formattime);\
		fprintf(escpplib::g_escpplib_log_stderr,"[DEBUG][%s][%s:%d#%s]" format "\n", formattime, __FILE__, __LINE__, __FUNCTION__, ##args);\
	}while(0)

#define LOG_DEBUG_MSG(format, args...) \
	do{\
		char formattime[18];\
		escpplib::getLogTime(formattime);\
		fprintf(escpplib::g_escpplib_log_stderr,"[DEBUG][%s][%s:%d#%s][errmsg:%s]" format "\n", formattime, __FILE__, __LINE__, __FUNCTION__, strerror(errno), ##args);\
	}while(0)


#define LOG_WORNING(format, args...) \
	do{\
		char formattime[18];\
		escpplib::getLogTime(formattime);\
		fprintf(escpplib::g_escpplib_log_stderr,"[WORNING][%s][%s:%d#%s]" format "\n", formattime, __FILE__, __LINE__, __FUNCTION__, ##args);\
	}while(0)



#define LOG_ERROR(format, args...) \
	do{\
		char formattime[18];\
		escpplib::getLogTime(formattime);\
		fprintf(escpplib::g_escpplib_log_stderr,"[ERROR][%s][%s:%d#%s]" format "\n", formattime, __FILE__, __LINE__, __FUNCTION__, ##args);\
	}while(0)

#define LOG_ERROR_MSG(format, args...) \
	do{\
		char formattime[18];\
		escpplib::getLogTime(formattime);\
		fprintf(escpplib::g_escpplib_log_stderr,"[ERROR][%s][%s:%d#%s][errmsg:%s]" format "\n", formattime, __FILE__, __LINE__, __FUNCTION__, strerror(errno), ##args);\
	}while(0)


#define LOG_FATAL(format, args...) \
	do{\
		char formattime[18];\
		escpplib::getLogTime(formattime);\
		fprintf(escpplib::g_escpplib_log_stderr,"[FATAL][%s][%s:%d#%s]" format "\n", formattime, __FILE__, __LINE__, __FUNCTION__, ##args);\
		_exit(1);\
	}while(0)

#define LOG_FATAL_MSG(format, args...) \
	do{\
		char formattime[18];\
		escpplib::getLogTime(formattime);\
		fprintf(escpplib::g_escpplib_log_stderr,"[FATAL][%s][%s:%d#%s][errmsg:%s]" format "\n", formattime, __FILE__, __LINE__, __FUNCTION__, strerror(errno), ##args);\
		_exit(1);\
	}while(0)



#endif
