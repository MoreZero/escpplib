#include "log/log.h"
#include "timer/timer.h"
#include <string> 
//using namespace escpplib;

int main(){
    std::string msg="dd";
    escpplib::CCostTimer timer(&msg); 
    LOG_INFO("time cost test");

    return 0;
}
