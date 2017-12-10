#ifndef ESCPPLIB_TIMER_H
#define ESCPPLIB_TIMER_H

#include <stdint.h>
#include <sys/time.h>
#include <string>

namespace escpplib{

class CCostTimer
{
public:
    CCostTimer(const char *msg, bool start=true);
    void Begin();
    int64_t Cost();
    ~CCostTimer();
private:
    CCostTimer();

    struct timeval  start_;
    std::string msg_;
    bool begin_;
};


}// namespace escpplib

#endif
