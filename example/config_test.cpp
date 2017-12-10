#include "log.h"
#include "timer/timer.h"
#include "config.h"
#include <string> 
//using namespace escpplib;

int main(){
    escpplib::CCostTimer timer("config test"); 
    LOG_INFO("config test");

    escpplib::CConfig  config;
    int32_t ret = config.ReadConfig("./test.ini");
    if (ret != 0) {
        LOG_FATAL("ReadConfig error");
    }

    std::string name = config.Get("test_section", "name", "3");
    uint32_t age = config.Get("test_section", "age", 30);
    uint32_t xx = config.Get("test_section", "xx", 3);
    
    LOG_INFO("name:%s age:%u xx:%u", name.c_str(), age,xx);

    return 0;
}
