#include <iostream>
#include "log.h"


using namespace std;

int main(){
    LOG_INFO("test");
    LOG_INFO("test %d", 1);
    LOG_ERROR("test %d", 1);
    LOG_ERROR_MSG("test %d", 1);
    LOG_DEBUG("test %d", 1);
    LOG_FATAL("test %d", 1);

    return 0;
}


