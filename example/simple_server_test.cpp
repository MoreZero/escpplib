#include <iostream>
#include <vector>
#include "log.h"
#include "timer/timer.h"
#include "simple_server.h"
#include "config.h"


using namespace std;



int main(){
    escpplib::CCostTimer timer("SimpleServer test"); 
    LOG_INFO("SimpleServer test");


    escpplib::CConfig  config;
    int32_t ret = config.ReadConfig("./test.ini");
    if (ret != 0) {
        LOG_FATAL("ReadConfig error");
    }

    escpplib::CSimpleServer simple_server;
    simple_server.Init("simple_server", config);

    ret = simple_server.Run();
    LOG_DEBUG_MSG("simple_server exit, ret:%d", ret); 

    return 0;
}


