#include "log.h"
#include "es_udp_socket.h"
#include "timer/timer.h"
#include <string> 
//using namespace escpplib;

int main(){
    escpplib::CCostTimer timer(); 
    LOG_INFO("socket test");

    escpplib::CUdpSocket udp_socket;
    //if (0 != udp_socket.Bind("119.29.155.157", 8000)) {
    if (0 != udp_socket.Bind("127.0.0.1", 8000)) {
        LOG_FATAL_MSG("udp_socket.Bind");
    }

    while(1) {
        char * buff;
        ssize_t cnt = udp_socket.RecvFrom(buff);
        if (cnt == -1) {
            LOG_ERROR_MSG("udp_socket.RecvFrom");
            continue;
        }
        buff[cnt] = 0;
        LOG_INFO("recv msg:%s", buff);
    }

    return 0;
}
