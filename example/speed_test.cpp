#include "log.h"
#include "timer/timer.h"
#include <string> 
#include <stdint.h> 
//using namespace escpplib;

int main(){
    LOG_INFO("speed test");
    int x=0;
    {
        escpplib::CCostTimer timer1("test1");
        for (uint64_t i=0; i<10000000; i++) {
            for (uint64_t j=0; j<1000; j++) {
                x++;
            }
        }
    }

    return 0;
}
