#include "timer/timer.h"
#include "log.h"

namespace escpplib{

CCostTimer::CCostTimer(std::string *msg, bool start):m_p_msg(msg){
    if (start ) {
        gettimeofday(&m_start, NULL);
        m_begin = true;
    }else {
        m_begin = false;
    }
}

void CCostTimer::Begin() {
    gettimeofday(&m_start, NULL);
    m_begin = true;
}

int64_t CCostTimer::Cost() {
    struct timeval end;
    gettimeofday(&end, NULL);
    m_begin = false;
    return ( 
        (int64_t)end.tv_sec * 1000000 + (int64_t)end.tv_usec
        -
        (int64_t)m_start.tv_sec * 1000000 + (int64_t)m_start.tv_usec
    );

}

CCostTimer::~CCostTimer(){
    if (m_begin){
        struct timeval end;
        gettimeofday(&end, NULL);

        if (m_p_msg == NULL) {
            LOG_INFO("cost time:%ld[us]", 
                (int64_t)end.tv_sec * 1000000 + (int64_t)end.tv_usec
                -
                (int64_t)m_start.tv_sec * 1000000 + (int64_t)m_start.tv_usec);
        } else {
            LOG_INFO("%s cost time:%ld[us]",
                m_p_msg->c_str(),
                (int64_t)end.tv_sec * 1000000 + (int64_t)end.tv_usec
                -
                (int64_t)m_start.tv_sec * 1000000 + (int64_t)m_start.tv_usec);
        }
    }
}


}// namespace escpplib

