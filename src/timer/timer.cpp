#include "timer/timer.h"
#include "log.h"

namespace escpplib{

CCostTimer::CCostTimer(const char *msg, bool start):msg_(std::string(msg)){
    if (start ) {
        gettimeofday(&start_, NULL);
        begin_ = true;
    }else {
        begin_ = false;
    }
}


void CCostTimer::Begin() {
    gettimeofday(&start_, NULL);
    begin_ = true;
}

int64_t CCostTimer::Cost() {
    struct timeval end;
    gettimeofday(&end, NULL);
    begin_ = false;
    return ( 
        ((int64_t)end.tv_sec * 1000000L + (int64_t)end.tv_usec)
        -
        ((int64_t)start_.tv_sec * 1000000L + (int64_t)start_.tv_usec)
    );

}

CCostTimer::~CCostTimer(){
    if (begin_){
        struct timeval end;
        gettimeofday(&end, NULL);
        LOG_DEBUG("curret tiem: sec:%ld usec:%ld", end.tv_sec, end.tv_usec);
        LOG_DEBUG("old tiem: sec:%ld usec:%ld", start_.tv_sec, start_.tv_usec);

        LOG_INFO("%s cost time:%ld[us]",
            msg_.c_str(),
            ((int64_t)end.tv_sec * 1000000L + (int64_t)end.tv_usec)
            -
            ((int64_t)start_.tv_sec * 1000000L + (int64_t)start_.tv_usec));
    }
}


}// namespace escpplib

