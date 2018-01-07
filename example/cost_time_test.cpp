#include "log.h"
#include "timer/timer.h"
#include <string> 
//using namespace escpplib;

int main(){
    LOG_INFO("time cost test");
    escpplib::CCostTimer timer1("test1"); 
    escpplib::CCostTimer timer2(""); 
    sleep(1);
    return 0;
}
