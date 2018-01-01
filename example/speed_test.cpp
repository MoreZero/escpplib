#include "log.h"
#include "timer/timer.h"
#include <string> 
#include <stdint.h> 
//using namespace escpplib;

int main(){
    LOG_INFO("speed test");
    {
        escpplib::CCostTimer timer1("test1");
        for (uint64_t i=0; i<1000000000; i++) {
            i++;
        }
    }

    return 0;
}
