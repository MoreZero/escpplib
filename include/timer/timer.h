#ifndef ESCPPLIB_TIMER_H
#define ESCPPLIB_TIMER_H

#include <stdint.h>
#include <sys/time.h>
#include <string>

namespace escpplib{

class CCostTimer
{
public:
    CCostTimer(std::string *msg, bool start=true);
    void Begin();
    int64_t Cost();
    ~CCostTimer();
private:
    CCostTimer();

    struct timeval  m_start;
    std::string *m_p_msg;
    bool m_begin;
};


}// namespace escpplib

#endif
